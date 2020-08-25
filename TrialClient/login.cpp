#include "login.h"
#include "ui_login.h"


Login::Login(Client *cl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login),
    client(cl)
{
    ui->setupUi(this);
    connect(client, SIGNAL(loginJsonSignal()), this, SLOT(loginSignalReceived()));
}

Login::~Login()
{
    delete ui;
}

void Login::loginSignalReceived(std::string status)
{
    if(status == "Success")
        emit logInSuccess();
    else
        ui->statusLabel->setText("Wrong Password");
}

void Login::on_loginButton_clicked()
{
    QString login = ui->loginLine->text();
    QString password = ui->passwordLine->text();

    client->sendLoginMessage(login.toStdString(), password.toStdString());

}

