#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "client.h"
#include "dialog.h"

namespace Ui {
class Login;
}

class Login: public QDialog
{
    Q_OBJECT

public:
    explicit Login(Client *cl, QWidget *parent = nullptr);
    ~Login();

signals:
    void logInSuccess();

private slots:

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
    Client *client;

};

#endif // LOGIN_H
