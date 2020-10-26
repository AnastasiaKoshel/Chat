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

    void sendTextMessage(const QString& text, const QString& login, const QString& chatLogin);
    void sendFileMessage(QString & filePath, const QString& login, const QString& chatLogin);
    void sendLoginMessage(const QString& login, const QString& password);
    void sendNewAccountMessage(const QString& login, const QString& password);
    void requestAllUsers();
    void getSelectedChat(const QString& login, const QString& chatLogin);
    const QString pathToSaveFiles = "C:/Users/Anastasiia_Koshel/Documents/Chat/";



signals:
    void processMessageSignal(const QString& message, const QString& recipientLogin);
    void processFileMessageSignal(const QString& fileName, const QString& file);
    void loginJsonSignal(const QString& status);
    void newAccountSignal(const QString& status);
    void userListReceived(const QJsonArray& usersArray);
    void userIdbyLoginSignal(const int myId, const int otherId);
    void sendJSON(QJsonObject& json);
    void sendFile(QFile& file);
    void fileSavedSignal(const QString& filePath);

public slots:
    void processJson(QJsonObject& object);
    void saveFile(const QString& fileName, const QString& file);

};

#endif // MESSAGEPARSER_H
