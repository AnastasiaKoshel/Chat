#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <string>
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
    void sendTextMessage(std::string text);
    void sendLoginMessage(std::string login, std::string password);
    void sendNewAccountMessage(std::string login, std::string password);
    void getRequestedChat(std::string senderLogin, std::string recipientLogin);
    void processMessage();
    void processJson();
    void requestAllUsers();

    void setLogin(const std::string login)  {myLogin = login;}
    std::string getLogin() const {return myLogin;}
    void setCurrentChatLogin(const std::string login ) {currentChatLogin = login;}
    std::string getCurrentChatLogin() const { return currentChatLogin;}

    void getSelectedChat();
    std::string messageCur;
    std::vector<std::string> contactsList;


signals:
    void processMessageSignal(std::string message, std::string recipientLogin);
    void disconnected();
    void loginJsonSignal(std::string status);
    void newAccountSignal(std::string status);
    void userListReceived(const QJsonArray& usersArray);
    void userIdbyLoginSignal(int myId, int otherId);


public slots:
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);


private:
    std::unique_ptr<QTcpSocket> tcpSocket;
    bool isLoggedIn = false;
    std::string myLogin;
    std::string currentChatLogin;


};



//! [0]

#endif
