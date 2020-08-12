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
   std::string messageCur;

private slots:
    void processMessage();

signals:
    void processMessageSignal(std::string s);

private:
    QTcpServer *tcpServer = nullptr;
    QSet<QTcpSocket*> clients;

};
//! [0]

#endif
