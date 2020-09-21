#include "dbmanager.h"

namespace
{
    const char* addClientQuery = "INSERT INTO clientData (id, login, password) VALUES (NULL, :login, :password)";
    const char* getAllUsersQuery = "SELECT * FROM clientData";
    const char* selectClientByLoginQuery = "SELECT * FROM clientData WHERE login = (:login)";
}

DBManager::DBManager()
{

    db = QSqlDatabase::addDatabase("QSQLITE", "ClientData");
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

bool DBManager::addClient(const QString& login, QString password)
{
    if(login.isEmpty() || password.isEmpty())
        return false;

    password = encryptPassword(password);
    bool success = false;
    // TODO: you should check if args are ok first...
    QSqlQuery query(db);
    query.prepare(addClientQuery);
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":id", 0);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "[DataBase] addPerson error:  "
                  << query.lastError();
    }
    return success;
}
std::vector<QString> DBManager::getAllUsers()
{
    std::vector<QString> allLogins;
    QSqlQuery query(db);
    query.prepare(getAllUsersQuery);

    if(!query.exec())
    {
        qDebug() << "[DataBase] Get All Users fro DB error:  "
                 << query.lastError();
        return allLogins;
    }

    const int loginInd = query.record().indexOf("login");
    while (query.next())
    {
       const QString name = query.value(loginInd).toString();
       allLogins.push_back(name);

       qDebug() << name;
    }
    return allLogins;
}

int DBManager::getIDbyLogin(const QString& login)
{
    QSqlQuery query(db);
    query.prepare(selectClientByLoginQuery);
    query.bindValue(":login", login);

    if( !query.exec() )
    {
        qDebug() << "[DataBase] Error getting user info from DB" << query.lastError();
        return 0;
    }

    const int loginInd = query.record().indexOf("login");
    const int passwordInd = query.record().indexOf("password");
    while(query.next())
    {
        qDebug() << "Login: " << query.value(loginInd).toString();
        qDebug() << "Password: " << query.value(passwordInd).toString();
        return query.value(0).toInt();;
    }
    return 0;
}

bool DBManager::loginAndPasswordMatch(const QString& login, const QString& password)
{
    QSqlQuery query(db);
    query.prepare(selectClientByLoginQuery);
    query.bindValue(":login", login);

    if( !query.exec() )
    {
        qDebug() << "Error getting user info from DB";
        return false;
    }

    const int loginInd = query.record().indexOf("login");
    const int passwordInd = query.record().indexOf("password");

    while(query.next())
    {
        qDebug() << "Login from DB: " << query.value(loginInd).toString()<< " Login income: "<<login;
        qDebug() << "Password: " << query.value(passwordInd).toString()<<" Password income: "<<password;

        const QString curPassword = decryptPassword(query.value(passwordInd).toString());

        if(curPassword == password )
            return true;
    }
    return false;
}

QString DBManager::encryptPassword(QString password)
{

    qDebug()<<"incomming password in encrypt "<<password;
    int step=3;
    std::string result;
    for(char cur : password.toStdString())
    {
        int curInt = cur - '0';
        curInt += step;
        curInt = curInt % 128;
        result += curInt + '0';

    }
    qDebug()<<"outcomming password in encrypt "<<result.c_str();
    return result.c_str();
}

QString DBManager::decryptPassword(QString password)
{
    qDebug()<<"incomming password in decrypt "<<password;
    const int step=3;
    std::string result;
    for(char cur : password.toStdString())
    {
        int curInt = cur - '0';
        curInt -= step;
        curInt = curInt % 128;
        result += curInt + '0';

    }
    qDebug()<<"outcomming password in decrypt "<<result.c_str();
    return result.c_str();

}

