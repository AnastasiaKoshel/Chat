#ifndef MESSAGESDBMANAGER_H
#define MESSAGESDBMANAGER_H
#include <QtSQL>

class MessagesDBManager
{
public:
    MessagesDBManager();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/messageData.db";

private:
    QSqlDatabase messagesDB;
};

#endif // MESSAGESDBMANAGER_H
