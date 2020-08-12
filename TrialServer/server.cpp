#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include "dialog.h"
#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{

}

bool Server::initServer()
{
    tcpServer = new QTcpServer(this);
    bool success = tcpServer->listen(QHostAddress::Any, 1234);

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::initClient);

    if (!success) {
        return false;
    }
    return true;
}

void Server::initClient()
{
    QTcpSocket *client =  tcpServer->nextPendingConnection();
    clients.insert(client);

    emit processMessageSignal("New client from:" + client->peerAddress().toString().toStdString());
    connect(client, &QIODevice::readyRead,
            this, &Server::processMessage);

    connect(client, &QAbstractSocket::disconnected,
            client, &QObject::deleteLater);
}

void Server::processMessage()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    char * message = new char[100];
    client->read(message, 100);

    foreach(QTcpSocket *clientCur, clients)
    {
        if(clientCur->socketDescriptor()!=client->socketDescriptor())
            clientCur->write(message);
    }

    emit processMessageSignal(message);
}

