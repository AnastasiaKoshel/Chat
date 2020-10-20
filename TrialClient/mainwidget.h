#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <string>
#include "newaccount.h"
#include "login.h"
#include "dialog.h"
#include "client.h"
#include <QDialog>
#include "messageparser.h"


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
    MessageParser *messageParser;

private slots:
    void on_mainLogInButton_clicked();
    void showDialog(const QJsonArray& userArray);
    void requestUserList(const QString& login);

    void on_newAccountButton_clicked();

private:
    std::unique_ptr<Ui::MainWidget> ui;
    Dialog *dialog;
    Login *login;
    NewAccount *newAccount;
    QString loginStr;
};

#endif // MAINWIDGET_H
