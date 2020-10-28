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
        case FILE_MESSAGE:
            processFile(object, client, clients);
            break;
    }

}
void MessageParser::processUsersList(QTcpSocket* sender)
{
    QJsonObject messageJson;
    messageJson["type"]=JSONType::USER_LIST;
    QJsonArray usersArray;
    std::vector<QString> usersVector = passwordLoginDB->getAllUsers();
    for(auto user : usersVector )
    {
         //QJsonObject userJson;
         usersArray.append(QJsonValue(user));
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
    qDebug()<<"Login" << login << " password"<<password;
    const bool ifLoginMatchPassword =
            passwordLoginDB->loginAndPasswordMatch(login.toString(), password.toString());

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
            curClient->login = login.toString();
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
    const int id = passwordLoginDB->getIDbyLogin(login.toString());

    QJsonObject messageJson;
    messageJson["type"] = JSONType::NEW_ACCOUNT;
    if(id)
    {
       messageJson["status"] = "Fail";
    }
    else
    {
       passwordLoginDB->addClient(login.toString(), password.toString());
       messageJson["status"] = "Success";
       //emit processMessageSignal("New Account has been created");
    }

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);

    for(auto curClient : clients)
    {
        if(curClient->clientSocket->socketDescriptor() == sender->socketDescriptor())
        {
            curClient->login = login.toString();
        }
    }

}
void MessageParser::processMessage(const QJsonObject& json, const std::vector<ClientData*>&clients)
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

void MessageParser::processFile(const QJsonObject& json, QTcpSocket* sender, const std::vector<ClientData*>&clients)
{
    qDebug()<<"File json";
    const QString file = json.value("value").toString();
    const QString senderLogin = json.value("senderLogin").toString();
    const QString recipientLogin = json.value("recipientLogin").toString();
    QTcpSocket* recepient; //TODO: fix this
    foreach(ClientData* clientCur, clients)
    {
        if(clientCur->login == recipientLogin)
        {
            recepient = clientCur->clientSocket;
            //emit processMessageSignal(message);
        }
    }
    //TODO: rewrite this
    fileManager = std::make_unique<FileManager>(/*sender,*/recepient, json.value("size").toInt(),
                                                json.value("senderLogin").toString(), json.value("fileName").toString());
    //connect(fileManager.get(), SIGNAL(sendFileSignal(QByteArray&)),
     //       this, SLOT(sendFileToServer(QByteArray&)));
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
