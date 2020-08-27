#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "newaccount.h"
#include "login.h"
#include "dialog.h"
#include "client.h"
#include <QDialog>


namespace Ui {
class MainWidget;
}

class MainWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    Client *client;

private slots:
    void on_mainLogInButton_clicked();
    void showDialog(QJsonArray userArray);
    void requestUserList();

    void on_newAccountButton_clicked();

private:
    Ui::MainWidget *ui;
    Dialog *dialog;
    Login *login;
    NewAccount *newAccount;
};

#endif // MAINWIDGET_H
