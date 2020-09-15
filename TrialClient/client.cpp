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
    connect(tcpSocket.get(), &QIODevice::readyRead, this, &Client::receiveJSON);

}

void Client::connectToServer()
{
    tcpSocket->connectToHost("localhost",1234);
    qDebug() << "Connect to Server";
}



void Client::receiveJSON()
{
    QByteArray jsonData = tcpSocket->read(1000);
    QJsonParseError parseError;

    const QJsonDocument json = QJsonDocument::fromJson(jsonData, &parseError);
    QJsonObject jsonObject = json.object();

    emit jsonReceived(jsonObject);
}

void Client::sendJSON(QJsonObject& json)
{
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    tcpSocket->write(jsonData);
}


//errors will be handeled later
void Client::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"[ERROR] SocketError: "<<socketError;
}



