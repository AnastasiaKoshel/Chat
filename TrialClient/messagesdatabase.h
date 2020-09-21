#ifndef MESSAGESDATABASE_H
#define MESSAGESDATABASE_H

#include <QtSQL>
#include <string>



struct Message
{
    QString text;
    bool isMyMessage;
    int timestamp;

};

class MessagesDataBase
{
public:
    MessagesDataBase();
    std::vector<Message> getMessageHistory(int myLogin, int otherLogin);

private:
    QSqlDatabase messagesDB;
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/messageData.db";
};

#endif // MESSAGESDATABASE_H
