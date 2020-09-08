#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H
#include <QDialog>
#include <string>
#include <QVector>
#include <QtNetwork>
#include <QtCore>

#include "vector"
#include <QSet>
#include "dbmanager.h"
#include "messagesdbmanager.h"




struct ClientData{

    QTcpSocket* clientSocket;
    std::string password;
    std::string login;
};

class MessageParser
{
public:
    MessageParser();



    void processLogin(const QJsonObject& json,  QTcpSocket* sender, std::vector<ClientData*>&clients);
    void processNewAccount(const QJsonObject& json,  QTcpSocket* sender, std::vector<ClientData*>&clients);
    void processMessage(const QJsonObject& json,  std::vector<ClientData*>&clients);
    void processUsersList(QTcpSocket* sender);
    void processUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender);

signals:
    void processLoginSignal(std::string s);

private:
    std::unique_ptr<DBManager> passwordLoginDB;
    std::unique_ptr<MessagesDBManager> messagesDB;

};

#endif // MESSAGEPARSER_H
