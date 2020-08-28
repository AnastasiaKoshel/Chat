#include "messagesdatabase.h"

MessagesDataBase::MessagesDataBase()
{
    messagesDB = QSqlDatabase::addDatabase("QSQLITE", "MessageDB");
    messagesDB.setDatabaseName(path);
    qDebug() << QSqlDatabase::drivers();
    if (!messagesDB.open())
    {
       qDebug() << "Error: connection with database fail";
    }
    else
    {
       qDebug() << "Database: connection ok";
    }
}

std::vector<Message> MessagesDataBase::getMessageHistory(int myID, int otherID)
{
    std::vector<Message> messageHistory;
    Message currentMessage;
    QSqlQuery query(messagesDB);
    query.prepare("SELECT * FROM messageData WHERE sender = (:sender) AND receiver = (:receiver)");
    query.bindValue(":sender", myID);
    query.bindValue(":receiver", otherID);
    if(!query.exec())
    {
        qDebug() << "Error getting message history info from DB";
        return messageHistory;
    }
    while(query.next())
    {
        currentMessage.text = query.value(3).toString().toStdString();
        currentMessage.timestamp = query.value(4).toInt();
        currentMessage.isMyMessage = true;
        messageHistory.push_back(currentMessage);
        qDebug() << "My mess "<<currentMessage.text.c_str();

    }

    query.prepare("SELECT * FROM messageData WHERE sender = (:receiver) AND receiver = (:sender)");
    query.bindValue(":sender", myID);
    query.bindValue(":receiver", otherID);
    if(!query.exec())
    {
        qDebug() << "Error getting message history2 info from DB";
        return messageHistory;
    }
    while(query.next())
    {
        currentMessage.text = query.value(3).toString().toStdString();
        currentMessage.timestamp = query.value(4).toInt();
        currentMessage.isMyMessage = false;
        messageHistory.push_back(currentMessage);
        qDebug() << "Other mess "<<currentMessage.text.c_str();

    }
    sort(messageHistory.begin(), messageHistory.end(),
        [](const Message& a, const Message& b) -> bool
    {
        return a.timestamp < b.timestamp;
    });

    return messageHistory;
}
