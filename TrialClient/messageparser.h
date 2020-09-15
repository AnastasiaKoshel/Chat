#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <string>
#include "client.h"

class MessageParser : public QObject
{
    Q_OBJECT

public:
    MessageParser(QObject  *parent = nullptr);
    Client *client;

    void sendTextMessage(const std::string& text, const std::string& login, const std::string& chatLogin);
    void sendLoginMessage(const std::string& login, const std::string& password);
    void sendNewAccountMessage(const std::string& login, const std::string& password);
    void requestAllUsers();
    void getSelectedChat(const std::string& login, const std::string& chatLogin);



signals:
    void processMessageSignal(const std::string& message, const std::string& recipientLogin);
    void loginJsonSignal(const std::string& status);
    void newAccountSignal(const std::string& status);
    void userListReceived(const QJsonArray& usersArray);
    void userIdbyLoginSignal(const int myId, const int otherId);
    void sendJSON(QJsonObject& json);

public slots:
    void processJson(QJsonObject& object);

};

#endif // MESSAGEPARSER_H
