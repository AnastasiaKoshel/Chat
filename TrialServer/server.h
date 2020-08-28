#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QtNetwork>
#include <QtCore>
#include "string"
#include "vector"
#include <QSet>
#include "dbmanager.h"
#include "messagesdbmanager.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
QT_END_NAMESPACE



struct ClientData{

    QTcpSocket* clientSocket;
    std::string password;
    std::string login;
};

//! [0]
class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    bool initServer();
    void initClient();

private slots:
    void jsonReceived();
    void deleteUser();

signals:
    void processMessageSignal(std::string s);

private:
    void messageReceived(const QJsonObject& json);
    void processLogin(const QJsonObject& json,  QTcpSocket* sender);
    void processNewAccount(const QJsonObject& json,  QTcpSocket* sender);
    void processMessage(const QJsonObject& json, QTcpSocket* sender);
    void sendUsersList(QTcpSocket* sender);
    void sendUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender);
    QTcpServer* tcpServer = nullptr;
    std::vector<ClientData*> clients;
    DBManager* db;
    MessagesDBManager* messagesDB;

};


//! [0]

#endif
