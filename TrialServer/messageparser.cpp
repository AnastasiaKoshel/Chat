#include "messageparser.h"
#include "jsonType.h"

MessageParser::MessageParser()
{
    passwordLoginDB = std::make_unique<DBManager>();
    messagesDB = std::make_unique<MessagesDBManager>();
}


void MessageParser::processJson(QJsonObject& object, QTcpSocket* client, const std::vector<ClientData*>& clients)
{
    QJsonValue action = object.value("type");
    qDebug()<<"jSonType "<<action;

    switch(JSONType(action.toInt()))
    {
        case LOGIN:
            processLogin(object, client, clients);
            break;
        case NEW_ACCOUNT:
            processNewAccount(object, client, clients);
            break;
        case USER_LIST:
            processUsersList(client);
            break;
        case USER_ID_BY_LOGIN:
            processUserIdbyLogin(object, client);
            break;
        case MESSAGE:
            processMessage(object, clients);
            break;
    }

}
void MessageParser::processUsersList(QTcpSocket* sender)
{
    QJsonObject messageJson;
    messageJson["type"]=JSONType::USER_LIST;
    QJsonArray usersArray;
    std::vector<std::string> usersVector = passwordLoginDB->getAllUsers();
    for(auto user : usersVector )
    {
         //QJsonObject userJson;
         usersArray.append(QJsonValue(user.c_str()));
    }
    messageJson["userList"]=usersArray;
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

}
void MessageParser::processLogin(const QJsonObject& json,  QTcpSocket* sender, const std::vector<ClientData*>&clients)
{
    qDebug()<<"Login json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");
    bool ifLoginMatchPassword =
            passwordLoginDB->loginAndPasswordMatch(login.toString().toStdString(), password.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = JSONType::LOGIN;
    if(ifLoginMatchPassword)
        messageJson["status"] = "Success";
    else
        messageJson["status"] = "Fail";

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);


    for(auto curClient : clients)
    {
        if(curClient->clientSocket->socketDescriptor() == sender->socketDescriptor())
        {
            curClient->login = login.toString().toStdString();
        }
    }
    //emit processMessageSignal("Login: " + login.toString().toStdString() + "  \nPassword: " + password.toString().toStdString());

}


void MessageParser::processNewAccount(const QJsonObject& json,  QTcpSocket* sender, const std::vector<ClientData*>&clients)
{
    qDebug()<<"New Account json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");


    //TODO: emit signal
    int id = passwordLoginDB->getIDbyLogin(login.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = JSONType::NEW_ACCOUNT;
    if(id)
    {
       messageJson["status"] = "Fail";
    }
    else
    {
       passwordLoginDB->addClient(login.toString().toStdString(), password.toString().toStdString());
       messageJson["status"] = "Success";
       //emit processMessageSignal("New Account has been created");
    }

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

    for(auto curClient : clients)
    {
        if(curClient->clientSocket->socketDescriptor() == sender->socketDescriptor())
        {
            curClient->login = login.toString().toStdString();
        }
    }

}
void MessageParser::processMessage(const QJsonObject& json, const std::vector<ClientData*>&clients)
{
    qDebug()<<"Message json";
    const std::string message = json.value("value").toString().toStdString();
    const std::string senderLogin = json.value("senderLogin").toString().toStdString();
    const std::string recipientLogin = json.value("recipientLogin").toString().toStdString();

    messagesDB->writeMessageToDB(message, passwordLoginDB->getIDbyLogin(senderLogin),passwordLoginDB->getIDbyLogin(recipientLogin));

    QJsonObject messageJson;
    messageJson["type"] = JSONType::MESSAGE;
    messageJson["text"] = message.c_str();
    messageJson["senderLogin"] = senderLogin.c_str();
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);

    //const int recipient = json.value("recipientID").toInt();
    foreach(ClientData* clientCur, clients)
    {
        if(clientCur->login == recipientLogin)
        {
            clientCur->clientSocket->write(jsonData);
            //emit processMessageSignal(message);
        }
    }

}

void MessageParser::processUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender)
{
    qDebug()<<"entered sendUserIdbyLogin";

    std::string login1 = json.value("myLogin").toString().toStdString();
    std::string login2 = json.value("otherLogin").toString().toStdString();
    int id1 = passwordLoginDB->getIDbyLogin(login1);
    int id2 = passwordLoginDB->getIDbyLogin(login2);

    qDebug()<<"Received logins "<<login1.c_str()<<" and "<<login2.c_str()<<" Return ids "<<id1<<" and "<<id2;

    QJsonObject messageJson;
    messageJson["type"] = JSONType::USER_ID_BY_LOGIN;
    messageJson["id1"] = id1;
    messageJson["id2"] = id2;

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);
}
