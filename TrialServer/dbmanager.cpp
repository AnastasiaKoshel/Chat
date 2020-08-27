#include "dbmanager.h"

DBManager::DBManager()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    qDebug() << QSqlDatabase::drivers();
    if (!db.open())
    {
       qDebug() << "Error: connection with database fail";
    }
    else
    {
       qDebug() << "Database: connection ok";
    }
}

bool DBManager::addClient(std::string login, std::string password)
{
    bool success = false;
    // you should check if args are ok first...
    QSqlQuery query;
    int id = 1;
    query.prepare("INSERT INTO clientData (id, login, password) VALUES (NULL, :login, :password)");
    query.bindValue(":login", login.c_str());
    query.bindValue(":password", password.c_str());
    query.bindValue(":id", 0);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "addPerson error:  "
                  << query.lastError();
    }
    // printAll();
    return success;
}
std::vector<std::string> DBManager::getAllUsers()
{
    std::vector<std::string> allLogins;
    QSqlQuery query("SELECT * FROM clientData");
    int idName = query.record().indexOf("login");
    while (query.next())
    {
       QString name = query.value(idName).toString();
       allLogins.push_back(name.toStdString());

       qDebug() << name;
    }
    return allLogins;
}

bool DBManager::loginPresent(std::string login)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM clientData WHERE login = (:login)");
    query.bindValue(":login", login.c_str());
    if( !query.exec() )
    {
        qDebug() << "Error getting user info from DB";
        return false;
    }
    while(query.next())
    {
        qDebug() << "Login: " << query.value(0).toString();
        qDebug() << "Password: " << query.value(1).toString();
        return true;
    }
    return false;
}

bool DBManager::loginAndPasswordMatch(std::string login, std::string password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM clientData WHERE login = (:login)");
    query.bindValue(":login", login.c_str());
    if( !query.exec() )
    {
        qDebug() << "Error getting user info from DB";
        return false;
    }
    while(query.next())
    {
        qDebug() << "Login from DB: " << query.value(1).toString()<< " Login income: "<<login.c_str();
        qDebug() << "Password: " << query.value(2).toString()<<" Password income: "<<password.c_str() ;

        if(query.value(2).toString().toStdString() == password )
            return true;
    }
    return false;
}
