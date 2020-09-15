#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSQL>

class DBManager
{
public:
    DBManager();
    const std::string path = "C:/Users/Anastasiia_Koshel/SQlite/clientData.db";
    bool addClient(const std::string login, std::string password);
    int getIDbyLogin(const std::string login);
    bool loginAndPasswordMatch(const std::string login, const std::string password);
    std::vector<std::string> getAllUsers();
    std::string decryptPassword(std::string password);
    std::string encryptPassword(std::string password);


private:
    QSqlDatabase db;

};

#endif // DBMANAGER_H
