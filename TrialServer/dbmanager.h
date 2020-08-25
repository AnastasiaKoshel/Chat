#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSQL>

class DBManager
{
public:
    DBManager();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/clientData.db";
    bool addClient(std::string login, std::string password);
    bool loginPresent(std::string login);
    bool loginAndPasswordMatch(std::string login, std::string password);
    bool printAll();

private:
    QSqlDatabase db;

};

#endif // DBMANAGER_H
