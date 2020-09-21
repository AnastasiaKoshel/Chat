#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSQL>

class DBManager
{
public:
    DBManager();
    const QString path = "C:/Users/Anastasiia_Koshel/SQlite/clientData.db";
    bool addClient(const QString& login, QString password);
    int getIDbyLogin(const QString& login);
    bool loginAndPasswordMatch(const QString& login, const QString& password);
    std::vector<QString> getAllUsers();
    QString decryptPassword(QString password);
    QString encryptPassword(QString password);


private:
    QSqlDatabase db;

};

#endif // DBMANAGER_H
