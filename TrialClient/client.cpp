#include <QtWidgets>
#include <QtNetwork>

#include "client.h"


//! [0]
Client::Client(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &Client::displayError);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::processJson);
}

void Client::connectToServer()
{
    tcpSocket->connectToHost("localhost",1234);
    qDebug() << "Connect to Server";
}

void Client::sendTextMessage(std::string text, int recipientID)
{
    qDebug() << "Send message";

    QJsonObject messageJson;
    messageJson["type"] = "message";
    messageJson["value"] = text.c_str();
    messageJson["recipientID"] = recipientID;

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
    else
    {
        std::string text= json.object().value("value").toString().toStdString();
        text+='\n';
        messageCur += text;
        emit processMessageSignal();
    }
}
