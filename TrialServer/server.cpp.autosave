#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include "dialog.h"
#include "server.h"
#include "jsonType.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{

}

bool Server::initServer()
{
    tcpServer = std::make_unique<QTcpServer>(this);
    bool success = tcpServer->listen(QHostAddress::Any, 1234);
    clients.reserve(1000);

    connect(tcpServer.get(), &QTcpServer::newConnection, this, &Server::initClient);

    parser = std::make_unique<MessageParser>();

    if (!success) {
        return false;
    }
    return true;
}

void Server::initClient()
{
    ClientData* client = new ClientData();
    client->clientSocket = tcpServer->nextPendingConnection();
    client->isFileTranmition = false;
    clients.push_back(client);

    emit processMessageSignal("New client from:" + QString::number(client->clientSocket->socketDescriptor()));
    connect(client->clientSocket, &QIODevice::readyRead,
            this, &Server::messageReceived);

    connect(client->clientSocket, &QAbstractSocket::disconnected,
            client->clientSocket, &QObject::deleteLater);
    connect(client->clientSocket, &QAbstractSocket::disconnected,
            this, &Server::deleteUser);

}
void Server::messageReceived()
{
    //TODO: rewrite this
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray data = client->readAll();
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i]->clientSocket->socketDescriptor() == client->socketDescriptor())
        {
            if(clients[i]->isFileTranmition)
            {
                qDebug()<<"Second File Message received";
                parser->passFileData(data);
                return;
            }
        }
    }

    QJsonParseError parseError;
    const QJsonDocument json = QJsonDocument::fromJson(data, &parseError);
    QJsonObject jsonObject = json.object();
    if(jsonObject.value("type")== JSONType::FILE_MESSAGE)
    {
        for (size_t i = 0; i < clients.size(); ++i)
        {
            if(clients[i]->clientSocket->socketDescriptor() == client->socketDescriptor())
            {
                qDebug()<<"File Message received";
                clients[i]->isFileTranmition = true;
            }
        }
    }

    parser->processJson(jsonObject, client, clients);

}



void Server::deleteUser()
{
    qDebug()<<"clients array size ="<<clients.size();
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i]->clientSocket->socketDescriptor() == ((QTcpSocket*)sender())->socketDescriptor())
        {
            clients.erase(clients.begin()+i);
            return;
        }
    }
}
