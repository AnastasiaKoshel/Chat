#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include "dialog.h"
#include "server.h"

Server::Server(QWidget *parent)
    : QDialog(parent)
{

}

bool Server::initServer()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 1234)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return false;
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::initClient);
    //connect(tcpServer,  &QTcpServer::newConnection, this, &Server::sendMessage);
    return true;
}
void Server::initClient()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    client = clientConnection;
    connect(clientConnection, &QIODevice::readyRead,
            this, &Server::processMessage);

    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
}
void Server::sendMessage(std::string message)
{
    client->write(message.data());
}
void Server::processMessage()
{
    char * message = new char[100];
    client->read(message, 100);
    std::string res(message);
    res+='\n';
    messageCur += res;
    emit processMessageSignal();
}

