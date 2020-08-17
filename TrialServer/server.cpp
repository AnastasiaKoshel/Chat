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
    ClientData* client = new ClientData();
    client->clientSocket = tcpServer->nextPendingConnection();
    clients.insert(client);

    emit processMessageSignal("New client from:" + std::to_string(client->clientSocket->socketDescriptor()));
    connect(client->clientSocket, &QIODevice::readyRead,
            this, &Server::jsonReceived);

    connect(client->clientSocket, &QAbstractSocket::disconnected,
            client->clientSocket, &QObject::deleteLater);
}



void Server::jsonReceived()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    QByteArray jsonData = client->read(1000);
    QJsonParseError parseError;

    const QJsonDocument json = QJsonDocument::fromJson(jsonData, &parseError);
    const QJsonValue type = json.object().value("type");
    qDebug()<<"jSonType "<<type;
    if(type == "login")
        processLogin(json.object(), client);
    else {
        processMessage(json.object(), client);
    }

}

void Server::processLogin(const QJsonObject& json,  QTcpSocket* sender)
{
    qDebug()<<"Login json";
    const QJsonValue login = json.value("login");
    const QJsonValue password = json.value("password");

    foreach(ClientData *clientCur, clients)
    {
        qDebug()<<clientCur->clientSocket->socketDescriptor();
        if(clientCur->clientSocket->socketDescriptor()==sender->socketDescriptor())
        {
            clientCur->login = login.toString().toStdString();
            clientCur->password = password.toString().toStdString();

            emit processMessageSignal("Login: " + clientCur->login + "  \nPassword: " + clientCur->password);
        }
    }

}
void Server::processMessage(const QJsonObject& json, QTcpSocket* sender)
{
 qDebug()<<"Message json";
    const QJsonValue messageText = json.value("value");
    const std::string message = messageText.toString().toStdString();
    foreach(ClientData* clientCur, clients)
    {
        if(clientCur->clientSocket->socketDescriptor()!=sender->socketDescriptor())
        {
            clientCur->clientSocket->write(message.c_str());
            emit processMessageSignal(message);
        }
    }

}

