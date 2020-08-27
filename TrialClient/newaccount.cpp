#include "newaccount.h"
#include "ui_newAccount.h"

NewAccount::NewAccount(Client *cl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAccount),
    client(cl)
{
    ui->setupUi(this);
    connect(client, SIGNAL(newAccountSignal(std::string)), this, SLOT(newAccountSignalReceived(std::string)));
}

NewAccount::~NewAccount()
{
    delete ui;
}

void NewAccount::newAccountSignalReceived(std::string status)
{
    if(status == "Success")
        emit createNewAccountSuccess();
    else
    {
        ui->statusLabel->setText("This login already exists");
        ui->loginLine->clear();
        ui->passwordLine1->clear();
        ui->passwordLine2->clear();
    }
}

void NewAccount::on_okButton_clicked()
{
    QString login = ui->loginLine->text();
    QString password1 = ui->passwordLine1->text();
    QString password2 = ui->passwordLine2->text();

    if(password1 != password2)
    {
        ui->statusLabel->setText("Passwords don`t match");
        return;
    }
    client->sendNewAccountMessage(login.toStdString(), password1.toStdString());

}
