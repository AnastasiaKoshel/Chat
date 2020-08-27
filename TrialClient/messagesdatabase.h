#ifndef MESSAGESDATABASE_H
#define MESSAGESDATABASE_H
#include <QtSQL>


struct Message
{
    std::string text;
    bool isMyMessage;
    int timestamp;

};

class MessagesDataBase
{
public:
    MessagesDataBase();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/messageData.db";
    std::vector<Message> getMessageHistory(int myLogin, int otherLogin);

private:
    QSqlDatabase messagesDB;
};

#endif // MESSAGESDATABASE_H
