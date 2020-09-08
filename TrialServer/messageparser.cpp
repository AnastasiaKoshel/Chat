#include "messageparser.h"

MessageParser::MessageParser()
{
    passwordLoginDB = std::make_unique<DBManager>();
    messagesDB = std::make_unique<MessagesDBManager>();
}
void MessageParser::processUsersList(QTcpSocket* sender)
{
    QJsonObject messageJson;
    messageJson["type"]="userList";
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
void MessageParser::processLogin(const QJsonObject& json,  QTcpSocket* sender, std::vector<ClientData*>&clients)
{
    qDebug()<<"Login json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");
    bool ifLoginMatchPassword =
            passwordLoginDB->loginAndPasswordMatch(login.toString().toStdString(), password.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = "login";
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


void MessageParser::processNewAccount(const QJsonObject& json,  QTcpSocket* sender, std::vector<ClientData*>&clients)
{
    qDebug()<<"New Account json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");


    //TODO: emit signal
    int id = passwordLoginDB->getIDbyLogin(login.toString().toStdString());

    QJsonObject messageJson;
    messageJson["type"] = "newAccount";
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
void MessageParser::processMessage(const QJsonObject& json, std::vector<ClientData*>&clients)
{
    qDebug()<<"Message json";
    const std::string message = json.value("value").toString().toStdString();
    const std::string senderLogin = json.value("senderLogin").toString().toStdString();
    const std::string recipientLogin = json.value("recipientLogin").toString().toStdString();

    messagesDB->writeMessageToDB(message, passwordLoginDB->getIDbyLogin(senderLogin),passwordLoginDB->getIDbyLogin(recipientLogin));

    QJsonObject messageJson;
    messageJson["type"] = "message";
    messageJson["text"] = message.c_str();
    messageJson["recipientLogin"] = recipientLogin.c_str();
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

    //qDebug()<<"Received logins "<<login1<<" and "<<login2<<" Return ids "<<id1<<" and "<<id2;

    QJsonObject messageJson;
    messageJson["type"] = "getUserIdbyLogin";
    messageJson["id1"] = id1;
    messageJson["id2"] = id2;

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    sender->write(jsonData);
}
