#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include "dialog.h"
#include "server.h"


Server::Server(QObject *parent)
    : QTcpServer(parent)
{

}

bool Server::initServer()
{
    tcpServer = new QTcpServer(this);
    bool success = tcpServer->listen(QHostAddress::Any, 1234);

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::initClient);

    db = new DBManager();
    messagesDB = new MessagesDBManager();


    if (!success) {
        return false;
    }
    return true;
}

void Server::initClient()
{
    ClientData* client = new ClientData();
    client->clientSocket = tcpServer->nextPendingConnection();
    clients.insert(client);

    emit processMessageSignal("New client from:" + std::to_string(client->clientSocket->socketDescriptor()));
    connect(client->clientSocket, &QIODevice::readyRead,
            this, &Server::jsonReceived);

    connect(client->clientSocket, &QAbstractSocket::disconnected,
            client->clientSocket, &QObject::deleteLater);
}



void Server::jsonReceived()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray jsonData = client->read(1000);
    QJsonParseError parseError;

    const QJsonDocument json = QJsonDocument::fromJson(jsonData, &parseError);
    const QJsonValue type = json.object().value("type");
    qDebug()<<"jSonType "<<type;
    if(type == "login")
        processLogin(json.object(), client);
    else if(type == "newAccount"){
        processNewAccount(json.object(), client);
    }
    else if(type == "getAllUsers"){
        sendUsersList(client);
    }
    else if(type == "getUserIdbyLogin")
    {
        sendUserIdbyLogin(json.object(), client);
    }
    else{
        processMessage(json.object(), client);
    }
}
void Server::sendUsersList(QTcpSocket* sender)
{
    QJsonObject messageJson;
    messageJson["type"]="userList";
    QJsonArray usersArray;
    std::vector<std::string> usersVector = db->getAllUsers();
    for(auto user : usersVector )
    {
         //QJsonObject userJson;
         usersArray.append(QJsonValue(user.c_str()));
    }
    messageJson["userList"]=usersArray;
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

}
void Server::processLogin(const QJsonObject& json,  QTcpSocket* sender)
{
    qDebug()<<"Login json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");
    bool ifLoginMatchPassword =
            db->loginAndPasswordMatch(login.toString().toStdString(), password.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = "login";
    if(ifLoginMatchPassword)
        messageJson["status"] = "Success";
    else
        messageJson["status"] = "Fail";

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

    emit processMessageSignal("Login: " + login.toString().toStdString() + "  \nPassword: " + password.toString().toStdString());

}


void Server::processNewAccount(const QJsonObject& json,  QTcpSocket* sender)
{
    qDebug()<<"New Account json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");

    int id = db->getIDbyLogin(login.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = "newAccount";
    if(id)
    {
       messageJson["status"] = "Fail";
    }
    else
    {
       db->addClient(login.toString().toStdString(), password.toString().toStdString());
       messageJson["status"] = "Success";
       emit processMessageSignal("New Account has been created");
    }


    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

}
void Server::processMessage(const QJsonObject& json, QTcpSocket* sender)
{
    qDebug()<<"Message json";
    const std::string message = json.value("value").toString().toStdString();
    const std::string senderLogin = json.value("senderLogin").toString().toStdString();
    const std::string recipientLogin = json.value("recipientLogin").toString().toStdString();

    messagesDB->writeMessageToDB(message, db->getIDbyLogin(senderLogin),db->getIDbyLogin(recipientLogin));

    QJsonObject messageJson;
    messageJson["type"] = "message";
    messageJson["value"] = message.c_str();
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);

    const int recipient = json.value("recipientID").toInt();
    foreach(ClientData* clientCur, clients)
    {
        if(clientCur->clientSocket->socketDescriptor()!=sender->socketDescriptor())
        {
            clientCur->clientSocket->write(jsonData);
            emit processMessageSignal(message);
        }
    }

}

void Server::sendUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender)
{
    qDebug()<<"entered sendUserIdbyLogin";

    std::string login1 = json.value("myLogin").toString().toStdString();
    std::string login2 = json.value("otherLogin").toString().toStdString();
    int id1 = db->getIDbyLogin(login1);
    int id2 = db->getIDbyLogin(login2);

    qDebug()<<"Received logins "<<login1.c_str()<<" and "<<login2.c_str()<<" Return ids "<<id1<<" and "<<id2;

    QJsonObject messageJson;
    messageJson["type"] = "getUserIdbyLogin";
    messageJson["id1"] = id1;
    messageJson["id2"] = id2;

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);
}
