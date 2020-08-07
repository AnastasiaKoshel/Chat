#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QtNetwork>
#include <QtCore>
#include "string"
QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
QT_END_NAMESPACE

//! [0]
class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    bool initServer();
   void initClient();
   std::string messageCur;
    void processMessage();

signals:
    void processMessageSignal();
public slots:
    void sendMessage(std::string message);


private:
    QTcpServer *tcpServer = nullptr;
    QTcpSocket* client;
    QDataStream in;


};
//! [0]

#endif
