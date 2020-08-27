#include "login.h"
#include "ui_login.h"


Login::Login(Client *cl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login),
    client(cl)
{
    ui->setupUi(this);
    connect(client, SIGNAL(loginJsonSignal(std::string)), this, SLOT(loginSignalReceived(std::string)));
}

Login::~Login()
{
    delete ui;
}

void Login::loginSignalReceived(std::string status)
{
    qDebug()<<"entered loginSignalReceived in Login class";
    if(status == "Success")
    {
        qDebug()<<"My login is: "<< ui->loginLine->text();
        client->setLogin(ui->loginLine->text().toStdString());
        emit logInSuccess();
    }
    else
    {
        ui->statusLabel->setText("Wrong Password");
        ui->loginLine->clear();
        ui->passwordLine->clear();
    }
}

void Login::on_loginButton_clicked()
{
    QString login = ui->loginLine->text();
    QString password = ui->passwordLine->text();

    client->sendLoginMessage(login.toStdString(), password.toStdString());

}

