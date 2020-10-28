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
    bool isFileTranmition;
};

class MessageParser : public QTcpServer
{
public:
    MessageParser();



    void processLogin(const QJsonObject& json,  QTcpSocket* sender, const std::vector<ClientData*>&clients);
    void processNewAccount(const QJsonObject& json,  QTcpSocket* sender, const std::vector<ClientData*>&clients);
    void processMessage(const QJsonObject& json,  const std::vector<ClientData*>&clients);
    void processFile(const QJsonObject& json, QTcpSocket* sender, const std::vector<ClientData*>&clients);
    void processUsersList(QTcpSocket* sender);
    void processUserIdbyLogin(const QJsonObject& json, QTcpSocket* sender);
    void passFileData(QByteArray& data);


signals:
    void processLoginSignal(QString s);

public slots:
    void processJson(QJsonObject& object, QTcpSocket* client, const std::vector<ClientData*>& clients);
    void fileReceived(QByteArray& data);
private:
    std::unique_ptr<DBManager> passwordLoginDB;
    std::unique_ptr<MessagesDBManager> messagesDB;
    QByteArray fileCur;
    std::unique_ptr<FileManager> fileManager;

};

#endif // MESSAGEPARSER_H
