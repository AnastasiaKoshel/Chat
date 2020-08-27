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
    bool ifLoginExists = db->loginPresent(login.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = "newAccount";
    if(ifLoginExists)
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
    const QJsonValue messageText = json.value("value");
    const std::string message = messageText.toString().toStdString();
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

