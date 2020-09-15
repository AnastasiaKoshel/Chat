#include "messageparser.h"
#include "jsonType.h"

MessageParser::MessageParser(QObject *parent)
    : QObject(parent),
    client(new Client())
{
    client->connectToServer();
    connect(client, SIGNAL(jsonReceived(QJsonObject&)), this, SLOT(processJson(QJsonObject&)));
    connect(this, SIGNAL(sendJSON(QJsonObject&)), client, SLOT(sendJSON(QJsonObject&)));
}

void MessageParser::sendTextMessage(std::string text, std::string login, std::string chatLogin)
{
    qDebug() << "Send message";

    QJsonObject messageJson;
    messageJson["type"] = JSONType::MESSAGE;
    messageJson["value"] = text.c_str();
    messageJson["recipientLogin"] = chatLogin.c_str();
    messageJson["senderLogin"] = login.c_str();

    emit sendJSON(messageJson);
}

void MessageParser::sendLoginMessage(std::string login, std::string password)
{
    qDebug() << "Send login message";

    QJsonObject messageJson;
    messageJson["type"] = JSONType::LOGIN;
    messageJson["login"] = login.c_str();
    messageJson["password"] = password.c_str();


    emit sendJSON(messageJson);
}

void MessageParser::sendNewAccountMessage(std::string login, std::string password)
{
    qDebug() << "Send login message";

    QJsonObject messageJson;
    messageJson["type"] = JSONType::NEW_ACCOUNT;
    messageJson["login"] = login.c_str();
    messageJson["password"] = password.c_str();

    emit sendJSON(messageJson);
}


void MessageParser::getSelectedChat(std::string login, std::string chatLogin)
{
    qDebug() << "Entered getSelectedChat and my login is "<<login.c_str();

    QJsonObject messageJson;
    messageJson["type"] = JSONType::USER_ID_BY_LOGIN;
    messageJson["myLogin"] = login.c_str();
    messageJson["otherLogin"] = chatLogin.c_str();

    emit sendJSON(messageJson);
}

void MessageParser::requestAllUsers()
{
    QJsonObject messageJson;
    messageJson["type"] =JSONType::USER_LIST;
    emit sendJSON(messageJson);
}

void MessageParser::processJson(QJsonObject& object)
{
    QJsonValue action = object.value("type");
    qDebug()<<"jSonType "<<action;
    switch(JSONType(action.toInt()))
    {
        case LOGIN:
            emit loginJsonSignal(object.value("status").toString().toStdString());
            break;
        case NEW_ACCOUNT:
            emit newAccountSignal(object.value("status").toString().toStdString());
            break;
        case USER_LIST:
            emit userListReceived(object.value("userList").toArray());
            break;
        case USER_ID_BY_LOGIN:
            emit userIdbyLoginSignal(object.value("id1").toInt(),
                                     object.value("id2").toInt());
            break;
        case MESSAGE:
            emit processMessageSignal(object.value("text").toString().toStdString(),
                                      object.value("senderLogin").toString().toStdString());
            break;
    }


    //TODO::add error
}

