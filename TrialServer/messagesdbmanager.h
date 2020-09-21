#ifndef MESSAGESDBMANAGER_H
#define MESSAGESDBMANAGER_H
#include <QtSQL>
#include <QDateTime>

class MessagesDBManager
{
public:
    MessagesDBManager();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/messageData.db";
    bool writeMessageToDB(const QString& message, const int senderID, const int recipientID);

private:
    QSqlDatabase messagesDB;
};

#endif // MESSAGESDBMANAGER_H
