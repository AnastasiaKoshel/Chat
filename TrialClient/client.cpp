#include <QtWidgets>
#include <QtNetwork>

#include "client.h"


//! [0]
Client::Client(QWidget *parent)
    : QDialog(parent)
    , portLineEdit(1234)
    , tcpSocket(new QTcpSocket(this))
{

    connect(tcpSocket, &QAbstractSocket::errorOccurred,

            this, &Client::displayError);
    connect(tcpSocket, &QIODevice::readyRead,
            this, &Client::processMessage);
}

void Client::sendMessage(std::string message)
{
    tcpSocket->abort();
    tcpSocket->connectToHost("localhost",
                             1234);
    tcpSocket->write(message.data());
}


void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
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
