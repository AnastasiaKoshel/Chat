#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <QString>
#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>

#include <vector>
#include <QJsonArray>
#include "messagesdatabase.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
QT_END_NAMESPACE

//! [0]
//!
//!


class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject  *parent = nullptr);
    void receiveJSON();
    std::unique_ptr<QTcpSocket> tcpSocket;
    bool isFileTransmition = false;

signals:
    void disconnected();
    void jsonReceived(QJsonObject& object);
    void fileDataReceived(QByteArray& data);


public slots:
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void sendJSON(QJsonObject& json);
    void sendFileData(QByteArray& data);


private:


};



//! [0]

#endif
