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
        ui->statusLabel->setText("This login Already exists");
}

void NewAccount::on_okButton_clicked()
{
    QString login = ui->loginLine->text();
    QString password = ui->passwordLine1->text();

    client->sendNewAccountMessage(login.toStdString(), password.toStdString());
    //emit logInSuccess();

}
