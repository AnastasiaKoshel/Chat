#include "messageparser.h"
#include "jsonType.h"
#include <utility>

MessageParser::MessageParser()
{
    passwordLoginDB = std::make_unique<DBManager>();
    messagesDB = std::make_unique<MessagesDBManager>();
    fileManager = std::make_unique<FileManager>();

}


void MessageParser::processJson(QJsonObject& object, QTcpSocket* client, const std::map<int, std::shared_ptr<ClientData>>& clients)
{
    QJsonValue action = object.value("type");
    qDebug()<<"jSonType "<<action;

    //TODO: patern command
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
            processMessage(object);
            break;
        case FILE_MESSAGE:
            processFile(object);
            break;
    }

}
void MessageParser::processUsersList(QTcpSocket* sender)
{
    QJsonObject messageJson;
    messageJson["type"]=JSONType::USER_LIST;
    QJsonArray usersArray;
    std::vector<QString> usersVector = passwordLoginDB->getAllUsers();
    for(const auto& user : usersVector )
    {
         usersArray.append(QJsonValue(user));
    }
    messageJson["userList"]=usersArray;
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);

    sender->write(jsonData);
}
void MessageParser::processLogin(const QJsonObject& json,  QTcpSocket* sender, const std::map<int, std::shared_ptr<ClientData>>&clients)
{
    const QString login = json.value("login").toString();
    const QString password = json.value("password").toString();
    qDebug()<<"[Message Parser] entered processLogin with login = "<< login << " and password = "<<password;

    const bool ifLoginMatchPassword =
            passwordLoginDB->loginAndPasswordMatch(login, password);

    QJsonObject messageJson;
    messageJson["type"] = JSONType::LOGIN;
    messageJson["status"] = ifLoginMatchPassword ? "Success" : "Fail";

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

    auto search = clients.find(sender->socketDescriptor());
    if(search != clients.end())
    {
        search->second->login = login;
        clientsLoginsMap.emplace(std::make_pair(login,std::weak_ptr<ClientData>(search->second)));
    }
}


void MessageParser::processNewAccount(const QJsonObject& json,  QTcpSocket* sender, const std::map<int, std::shared_ptr<ClientData>>&clients)
{
    qDebug()<<"New Account json";

    const QString login = json.value("login").toString();
    const QString password = json.value("password").toString();

    const int id = passwordLoginDB->getIDbyLogin(login);

    QJsonObject messageJson;
    messageJson["type"] = JSONType::NEW_ACCOUNT;
    messageJson["status"] = id ? "Fail" : "Success";
    if(!id)
    {
       passwordLoginDB->addClient(login, password);
    }

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

    auto search = clients.find(sender->socketDescriptor());
    if(search != clients.end())
    {
        search->second->login = login;
        std::weak_ptr<ClientData> clientData(search->second);
        clientsLoginsMap.emplace(std::make_pair(login, clientData));
    }

}
void MessageParser::processMessage(const QJsonObject& json)
{
    qDebug()<<"Message json";
    const QString message = json.value("value").toString();
    const QString senderLogin = json.value("senderLogin").toString();
    const QString recipientLogin = json.value("recipientLogin").toString();

    messagesDB->writeMessageToDB(message, passwordLoginDB->getIDbyLogin(senderLogin),passwordLoginDB->getIDbyLogin(recipientLogin));

    QJsonObject messageJson;
    messageJson["type"] = JSONType::MESSAGE;
    messageJson["text"] = message;
    messageJson["senderLogin"] = senderLogin;
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);

    auto search = clientsLoginsMap.find(recipientLogin);
    if(search != clientsLoginsMap.end() && !(search->second.expired()))
    {
        //std::shared_ptr<ClientData> cl(search->second);
        //        cl->clientSocket->write(jsonData);
        search->second.lock()->clientSocket->write(jsonData);
    }
}

void MessageParser::processFile(const QJsonObject& json)
{
    qDebug()<<"File json";
    const QString recipientLogin = json.value("recipientLogin").toString();
    QTcpSocket* recepient;

    auto search = clientsLoginsMap.find(recipientLogin);
    if(search != clientsLoginsMap.end() && !(search->second.expired()))
    {
        recepient = search->second.lock()->clientSocket;
    }
    else
    {
        //TODO: handle file sending if receiient is offline
        return;
    }
    fileManager->setReceipientSocket(recepient);
    fileManager->setSizeExpected (json.value("size").toInt());
    fileManager->setSenderLogin (json.value("senderLogin").toString());
    fileManager->setFileName(json.value("fileName").toString());
    fileManager->setCheckSum(json.value("checksum").toString());

}
void MessageParser::passFileData(QByteArray& data)
{
    qDebug()<<"[MessageParser] passing file data";
    fileManager->getFileData(data);
}


void MessageParser::processUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender)
{
    qDebug()<<"entered sendUserIdbyLogin";

    QString login1 = json.value("myLogin").toString();
    QString login2 = json.value("otherLogin").toString();
    int id1 = passwordLoginDB->getIDbyLogin(login1);
    int id2 = passwordLoginDB->getIDbyLogin(login2);

    qDebug()<<"Received logins "<<login1<<" and "<<login2<<" Return ids "<<id1<<" and "<<id2;

    QJsonObject messageJson;
    messageJson["type"] = JSONType::USER_ID_BY_LOGIN;
    messageJson["id1"] = id1;
    messageJson["id2"] = id2;

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);
}
