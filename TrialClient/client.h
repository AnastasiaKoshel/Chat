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
class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    void sendMessage(std::string message);
    void processMessage();
    std::string messageCur;

signals:
    void processMessageSignal();

public slots:

    void displayError(QAbstractSocket::SocketError socketError);

private:
    int portLineEdit = 1234;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString currentFortune;

};
//! [0]

#endif
