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

void MessageParser::sendTextMessage(const QString& text, const QString& login, const QString& chatLogin)
{
    qDebug() << "Send message";

    QJsonObject messageJson;
    messageJson["type"] = JSONType::MESSAGE;
    messageJson["value"] = text;
    messageJson["recipientLogin"] = chatLogin;
    messageJson["senderLogin"] = login;

    emit sendJSON(messageJson);
}

void MessageParser::sendLoginMessage(const QString& login, const QString& password)
{
    qDebug() << "Send login message";

    QJsonObject messageJson;
    messageJson["type"] = JSONType::LOGIN;
    messageJson["login"] = login;
    messageJson["password"] = password;


    emit sendJSON(messageJson);
}

void MessageParser::sendNewAccountMessage(const QString& login, const QString& password)
{
    qDebug() << "Send login message";

    QJsonObject messageJson;
    messageJson["type"] = JSONType::NEW_ACCOUNT;
    messageJson["login"] = login;
    messageJson["password"] = password;

    emit sendJSON(messageJson);
}


void MessageParser::getSelectedChat(const QString& login, const QString& chatLogin)
{
    qDebug() << "Entered getSelectedChat and my login is "<<login;

    QJsonObject messageJson;
    messageJson["type"] = JSONType::USER_ID_BY_LOGIN;
    messageJson["myLogin"] = login;
    messageJson["otherLogin"] = chatLogin;

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
            emit loginJsonSignal(object.value("status").toString());
            break;
        case NEW_ACCOUNT:
            emit newAccountSignal(object.value("status").toString());
            break;
        case USER_LIST:
            emit userListReceived(object.value("userList").toArray());
            break;
        case USER_ID_BY_LOGIN:
            emit userIdbyLoginSignal(object.value("id1").toInt(),
                                     object.value("id2").toInt());
            break;
        case MESSAGE:
            emit processMessageSignal(object.value("text").toString(),
                                      object.value("senderLogin").toString());
            break;
    }


    //TODO::add error
}

