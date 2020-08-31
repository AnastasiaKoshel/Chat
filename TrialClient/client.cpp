#include <QtWidgets>
#include <QtNetwork>

#include "client.h"


//! [0]
Client::Client(QObject *parent)
    : QObject(parent)
{
    tcpSocket = std::make_unique<QTcpSocket>(this);
    connect(tcpSocket.get(), &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(tcpSocket.get(), &QAbstractSocket::errorOccurred, this, &Client::displayError);
    connect(tcpSocket.get(), &QIODevice::readyRead, this, &Client::processJson);
}

void Client::connectToServer()
{
    tcpSocket->connectToHost("localhost",1234);
    qDebug() << "Connect to Server";
}

void Client::sendTextMessage(std::string text)
{
    qDebug() << "Send message";

    QJsonObject messageJson;
    messageJson["type"] = "message";
    messageJson["value"] = text.c_str();
    messageJson["recipientLogin"] = currentChatLogin.c_str();
    messageJson["senderLogin"] = myLogin.c_str();

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}

void Client::sendLoginMessage(std::string login, std::string password)
{
    qDebug() << "Send login message";

    QJsonObject messageJson;
    messageJson["type"] = "login";
    messageJson["login"] = login.c_str();
    messageJson["password"] = password.c_str();

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}
void Client::sendNewAccountMessage(std::string login, std::string password)
{
    qDebug() << "Send login message";

    QJsonObject messageJson;
    messageJson["type"] = "newAccount";
    messageJson["login"] = login.c_str();
    messageJson["password"] = password.c_str();

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}
//errors will be handeled later
void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    }
}

void Client::getRequestedChat(std::string senderLogin, std::string recipientLogin)
{
    qDebug() << "get Requested Chat for "<<senderLogin.c_str()<< " and "<<recipientLogin.c_str();

    QJsonObject messageJson;
    messageJson["type"] = "getChat";
    messageJson["senderLogin"] = senderLogin.c_str();
    messageJson["recipientLogin"] = recipientLogin.c_str();
}

void Client::processJson()
{
    QByteArray jsonData = tcpSocket->read(1000);
    QJsonParseError parseError;

    const QJsonDocument json = QJsonDocument::fromJson(jsonData, &parseError);
    const QJsonValue type = json.object().value("type");
    qDebug()<<"jSonType "<<type;
    if(type == "login")
    {
        emit loginJsonSignal(json.object().value("status").toString().toStdString());

    }
    else if(type == "newAccount"){
        emit newAccountSignal(json.object().value("status").toString().toStdString());
    }
    else if(type == "getUserIdbyLogin")
    {
        emit userIdbyLoginSignal(json.object().value("id1").toInt(),
                                 json.object().value("id2").toInt());
    }
    else if(type =="userList")
    {
        emit userListReceived(json.object().value("userList").toArray());
    }
    else
    {
        std::string text= json.object().value("value").toString().toStdString();
        text+='\n';
        messageCur += text;
        emit processMessageSignal(json.object().value("text").toString().toStdString(),
                                  json.object().value("recipientLogin").toString().toStdString());
    }
}
void Client::getSelectedChat()
{
    qDebug() << "Entered getSelectedChat and my login is "<<myLogin.c_str();

    QJsonObject messageJson;
    messageJson["type"] = "getUserIdbyLogin";
    messageJson["myLogin"] = myLogin.c_str();
    messageJson["otherLogin"] = currentChatLogin.c_str();

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}

void Client::requestAllUsers()
{
    QJsonObject messageJson;
    messageJson["type"] ="getAllUsers";
    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}

