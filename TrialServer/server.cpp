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
    clients.push_back(client);

    emit processMessageSignal("New client from:" + std::to_string(client->clientSocket->socketDescriptor()));
    connect(client->clientSocket, &QIODevice::readyRead,
            this, &Server::jsonReceived);

    connect(client->clientSocket, &QAbstractSocket::disconnected,
            client->clientSocket, &QObject::deleteLater);
    connect(client->clientSocket, &QAbstractSocket::disconnected,
            this, &Server::deleteUser);
}



void Server::jsonReceived()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray jsonData = client->read(1000);
    QJsonParseError parseError;

    const QJsonDocument json = QJsonDocument::fromJson(jsonData, &parseError);
    const QJsonValue action = json.object().value("type");
    qDebug()<<"jSonType "<<action;
    if(action == "login")
        parser->processLogin(json.object(), client, clients);
    else if(action == "newAccount"){
        parser->processNewAccount(json.object(), client, clients);
    }
    else if(action == "getAllUsers"){ //add enum and separate header
        parser->processUsersList(client);
    }
    else if(action == "getUserIdbyLogin")
    {
        parser->processUserIdbyLogin(json.object(), client);
    }
    else if(action == "message"){
        parser->processMessage(json.object(), clients);
    }
}


void Server::deleteUser()
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if(clients[i]->clientSocket->socketDescriptor() == ((QTcpSocket*)sender())->socketDescriptor())
        {
            clients.erase(clients.begin()+i-1, clients.begin()+i);
            return;
        }
    }
}
