#include "messagesdatabase.h"

MessagesDataBase::MessagesDataBase()
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

std::vector<std::pair<std::string, bool>> getMessageHistory()
{

}
