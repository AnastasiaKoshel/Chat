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
    //TODO: think about this
    tcpServer = std::make_unique<QTcpServer>(this);
    if (!tcpServer->listen(QHostAddress::Any, 1234))
    {
        return false;
    }
    connect(tcpServer.get(), &QTcpServer::newConnection, this, &Server::initClient);
    parser = std::make_unique<MessageParser>();

    return true;
}

void Server::initClient()
{
    auto client = std::make_shared<ClientData>(tcpServer->nextPendingConnection());

    clientsDescriptorsMap.emplace(std::make_pair(client->clientSocket->socketDescriptor(), client));

    emit processMessageSignal("New client from:" + QString::number(client->clientSocket->socketDescriptor()));
    connect(client->clientSocket, &QIODevice::readyRead,
            this, &Server::messageReceived);

    connect(client->clientSocket, &QAbstractSocket::disconnected,
            client->clientSocket, &QObject::deleteLater);
    connect(client->clientSocket, &QAbstractSocket::disconnected,
            this, &Server::removeUserFromList);

}
void Server::messageReceived()
{
    QTcpSocket *client = dynamic_cast<QTcpSocket*>(sender());
    QByteArray data = client->readAll();
    auto search = clientsDescriptorsMap.find(client->socketDescriptor());
    if(search != clientsDescriptorsMap.end())
    {
        if(search->second->isFileTranmition)
        {
            qDebug()<<"Second File Message received";
            parser->passFileData(data);
            return;
        }
    }

    QJsonParseError parseError;
    const QJsonDocument json = QJsonDocument::fromJson(data, &parseError);
    QJsonObject jsonObject = json.object();
    if(search != clientsDescriptorsMap.end() && jsonObject.value("type")== JSONType::FILE_MESSAGE)
    {
        qDebug()<<"File Message received";
        search->second->isFileTranmition = true;
    }
    parser->processJson(jsonObject, client, clientsDescriptorsMap);
}
//TODO: add hash_sum for file check


void Server::removeUserFromList()
{
    qDebug()<<"clients array size ="<<clientsDescriptorsMap.size();
    size_t numOfErasedObjects = clientsDescriptorsMap.erase(dynamic_cast<QTcpSocket*>(sender())->socketDescriptor());
    if(numOfErasedObjects>0)
        qDebug()<<"Client was removed from map"<<clientsDescriptorsMap.size();
}
