#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <string>
QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
QT_END_NAMESPACE

//! [0]
class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject  *parent = nullptr);
    void sendMessage(std::string message);
    void processMessage();
    std::string messageCur;

signals:
    void processMessageSignal();
    void disconnected();

public slots:
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket = nullptr;
    bool isLoggedIn = false;

};
//! [0]

#endif
