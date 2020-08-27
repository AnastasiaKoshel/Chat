#ifndef MESSAGESDATABASE_H
#define MESSAGESDATABASE_H
#include <QtSQL>

class MessagesDataBase
{
public:
    MessagesDataBase();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/messageData.db";

private:
    QSqlDatabase messagesDB;
};

#endif // MESSAGESDATABASE_H
