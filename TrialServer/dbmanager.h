#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSQL>

class DBManager
{
public:
    DBManager();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/clientData.db";
    bool addClient(std::string login, std::string password);
    int getIDbyLogin(std::string login);
    bool loginAndPasswordMatch(std::string login, std::string password);
    std::vector<std::string> getAllUsers();


private:
    QSqlDatabase db;

};

#endif // DBMANAGER_H
