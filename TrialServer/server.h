#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <string>
#include <QVector>
#include <QtNetwork>
#include <QtCore>
#include "vector"
#include <QSet>
#include "dbmanager.h"
#include "messagesdbmanager.h"
#include "messageparser.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
QT_END_NAMESPACE





//! [0]
class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    bool initServer();
    void initClient();

private slots:
    void messageReceived();
    void deleteUser();

signals:
    void processMessageSignal(QString s);

private:
    //void messageReceived(const QJsonObject& json);

    std::unique_ptr<QTcpServer> tcpServer;
    std::vector<ClientData*> clients;
    std::unique_ptr<MessageParser> parser;

};


//! [0]

#endif
