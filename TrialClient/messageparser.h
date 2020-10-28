#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <string>
#include "client.h"
#include "filemanager.h"

class MessageParser : public QObject
{
    Q_OBJECT

public:
    MessageParser(QObject  *parent = nullptr);
    Client *client;
    std::unique_ptr<FileManager> fileManager;

    void sendTextMessage(const QString& text, const QString& login, const QString& chatLogin);
    void sendFileMessage(QString & filePath, const QString& login, const QString& chatLogin);
    void sendLoginMessage(const QString& login, const QString& password);
    void sendNewAccountMessage(const QString& login, const QString& password);
    void requestAllUsers();
    void getSelectedChat(const QString& login, const QString& chatLogin);




signals:
    void processMessageSignal(const QString& message, const QString& recipientLogin);
    void processFileMessageSignal(const int size, const QString& fileName, const QString& file);
    void loginJsonSignal(const QString& status);
    void newAccountSignal(const QString& status);
    void userListReceived(const QJsonArray& usersArray);
    void userIdbyLoginSignal(const int myId, const int otherId);
    void sendJSON(QJsonObject& json);
    void sendFile(QByteArray& file);
    void fileSavedSignal(const QString& filePath);
    void receivingFileSignal();

public slots:
    void processJson(QJsonObject& object);
    void processFile(const int size, const QString& fileName, const QString& senderLogin);
    void receivedFileData(QByteArray& data);
    void fileSavedSlot(const QString& filePath);

};

#endif // MESSAGEPARSER_H
