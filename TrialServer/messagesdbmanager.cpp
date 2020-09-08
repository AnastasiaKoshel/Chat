#include "messagesdbmanager.h"

MessagesDBManager::MessagesDBManager()
{
    messagesDB = QSqlDatabase::addDatabase("QSQLITE", "MessageData");
    messagesDB.setDatabaseName(path.c_str());
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


bool MessagesDBManager::writeMessageToDB(std::string message, int senderID, int recipientID)
{
    //qDebug()<<"Entered writeMessageToDB message:"<<message<<" senderID:"<<senderID<<" recipientID:"<<recipientID;
    if(!senderID || !recipientID)
    {
        qDebug()<<"No such user";
        return false;
    }


    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QSqlQuery query(messagesDB);
    query.prepare("INSERT INTO messageData (sender, receiver, message, timestamp) "
                  "VALUES (:senderID, :recipientID, :message, :timestamp)");
    //query.bindValue(":id", 0);
    query.bindValue(":senderID", senderID);
    query.bindValue(":recipientID", recipientID);
    query.bindValue(":message", message.c_str());
    query.bindValue(":timestamp", timestamp);


    if(query.exec())
    {
        return true;
    }
    else
    {
         qDebug() << "insert message error:  "
                  << query.lastError();
    }
    // printAll();
    return false;
}
