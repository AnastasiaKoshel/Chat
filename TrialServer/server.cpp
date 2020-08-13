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
    //QTcpSocket *socket =
    ClientData* client = new ClientData();
    client->clientSocket = tcpServer->nextPendingConnection();
    clients.insert(client);

    emit processMessageSignal("New client from:" + std::to_string(client->clientSocket->socketDescriptor()));
    connect(client->clientSocket, &QIODevice::readyRead,
            this, &Server::processMessage);

    connect(client->clientSocket, &QAbstractSocket::disconnected,
            client->clientSocket, &QObject::deleteLater);
}

void Server::processMessage()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    char * message = new char[100];
    client->read(message, 100);
    std::string messageString(message);
    qDebug()<<messageString.data();
    if(messageString[0] == 'L')
    {
        int startLogin = messageString.find("Login:");
        int startPassword = messageString.find("Password:");
        qDebug()<<"startPass"<<startPassword;
        std::string log = messageString.substr(startLogin+5, startPassword-startLogin-5);
        std::string pass = messageString.substr(startPassword+9);
        foreach(ClientData *clientCur, clients)
        {
            qDebug()<<clientCur->clientSocket->socketDescriptor();
            if(clientCur->clientSocket->socketDescriptor()==client->socketDescriptor())
            {
                clientCur->login =log;
                clientCur->password = pass;

                emit processMessageSignal("Login: " + log + "  \nPassword: " + pass);
            }
        }
    }
    else
    {
        messageString.erase(0,1);
        foreach(ClientData* clientCur, clients)
        {
            if(clientCur->clientSocket->socketDescriptor()!=client->socketDescriptor())
            {
                clientCur->clientSocket->write(messageString.data());
                emit processMessageSignal(message);
            }
        }
    }


}

