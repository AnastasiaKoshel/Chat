#include <QtWidgets>
#include <QtNetwork>

#include "client.h"


//! [0]
Client::Client(QObject *parent)
: QObject(parent)
    , portLineEdit(1234)
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

void Client::sendMessage(std::string message)
{
    qDebug() << "Send message";
    tcpSocket->write(message.data());
}


void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    }
}

void Client::processMessage()
{
    char * message = new char[100];
    tcpSocket->read(message, 100);
    std::string res(message);
    res+='\n';
    messageCur += res;
    emit processMessageSignal();
}
