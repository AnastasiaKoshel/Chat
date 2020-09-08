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

    void sendTextMessage(std::string text, std::string login, std::string chatLogin);
    void sendLoginMessage(std::string login, std::string password);
    void sendNewAccountMessage(std::string login, std::string password);
    void requestAllUsers();
    void getSelectedChat(std::string login, std::string chatLogin);



signals:
    void processMessageSignal(std::string message, std::string recipientLogin);
    void loginJsonSignal(std::string status);
    void newAccountSignal(std::string status);
    void userListReceived(const QJsonArray& usersArray);
    void userIdbyLoginSignal(int myId, int otherId);
    void sendJSON(QJsonObject& json);

public slots:
    void processJson(QJsonObject& object);

};

#endif // MESSAGEPARSER_H
