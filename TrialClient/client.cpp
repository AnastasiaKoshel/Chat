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
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::processMessage);
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

    const QByteArray jsonData = QJsonDocument(messageJson).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}

void Client::sendLoginMessage(std::string login, std::string password)
{
    qDebug() << "Send message";

    QJsonObject messageJson;
    messageJson["type"] = "login";
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

void Client::processMessage()
{
    //vector<char>
    char * message = new char[100];
    const auto len = tcpSocket->read(message, 100);
    std::string res(message);//, message + len);
    delete[] message;
    res+='\n';
    messageCur += res;
    emit processMessageSignal();
}
