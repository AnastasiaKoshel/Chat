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
#include "filemanager.h"




struct ClientData{

    QTcpSocket* clientSocket;
    QString password;
    QString login;
    bool isFileTranmition = false;

    ClientData(QTcpSocket* socket) :
        clientSocket(socket)
    {

    }
};

class MessageParser : public QTcpServer
{
public:
    MessageParser();



    void processLogin(const QJsonObject& json,  QTcpSocket* sender, const std::map<int, std::shared_ptr<ClientData>>&clients);
    void processNewAccount(const QJsonObject& json,  QTcpSocket* sender, const std::map<int, std::shared_ptr<ClientData>>&clients);
    void processMessage(const QJsonObject& json);
    void processFile(const QJsonObject& json);
    void processUsersList(QTcpSocket* sender);
    void processUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender);
    void passFileData(QByteArray& data);


signals:
    void processLoginSignal(QString s);

public slots:
    void processJson(QJsonObject& object, QTcpSocket* client, const std::map<int, std::shared_ptr<ClientData>>& clients);
    void fileReceived(QByteArray& data);
private:
    std::unique_ptr<DBManager> passwordLoginDB;
    std::unique_ptr<MessagesDBManager> messagesDB;
    QByteArray fileCur;
    std::unique_ptr<FileManager> fileManager;
    std::map<QString, std::weak_ptr<ClientData>> clientsLoginsMap;

};

#endif // MESSAGEPARSER_H
