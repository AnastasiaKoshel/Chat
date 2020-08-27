#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <string>
#include <vector>
#include <QJsonArray>

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
struct Message
{
    std::string text;
    bool isMyMessage;
};

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject  *parent = nullptr);
    void sendTextMessage(std::string text, int recipientID);
    void sendLoginMessage(std::string login, std::string password);
    void sendNewAccountMessage(std::string login, std::string password);
    void getRequestedChat(std::string senderLogin, std::string recipientLogin);
    void processMessage();
    void processJson();
    void requestAllUsers();
    std::vector<Message> getSelectedChat(std::string myLogin, std::string otherLogin);
    std::string messageCur;
    std::vector<std::string> contactsList;
    int currentChatID;

signals:
    void processMessageSignal();
    void disconnected();
    void loginJsonSignal(std::string status);
    void newAccountSignal(std::string status);
    void userListReceived(QJsonArray usersArray);
    void userIdbyLoginSignal(int myId, int otherId);


public slots:
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);


private:
    QTcpSocket *tcpSocket = nullptr;
    bool isLoggedIn = false;


};



//! [0]

#endif
