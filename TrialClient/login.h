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
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    Client client;

private slots:

    void on_loginButton_clicked();

private:
    Ui::Login *ui;
    Ui::Dialog *dialog;
};

#endif // LOGIN_H
