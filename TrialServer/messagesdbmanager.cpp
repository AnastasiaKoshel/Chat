#include "messagesdbmanager.h"

MessagesDBManager::MessagesDBManager()
{
    messagesDB = QSqlDatabase::addDatabase("QSQLITE");
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

