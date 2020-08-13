#include "login.h"
#include "ui_login.h"

Login::Login(Client *cl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login),
    client(cl)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}


void Login::on_loginButton_clicked()
{
    QString login = ui->loginLine->text();
    QString password = ui->loginLine->text();
    QString messageForServer = "LLogin:"+login+"Password:"+password;
    client->sendMessage(messageForServer.toStdString());

    emit logInSuccess();
}
