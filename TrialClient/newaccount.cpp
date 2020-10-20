#include "newaccount.h"
#include "ui_newAccount.h"

NewAccount::NewAccount(MessageParser *msParser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAccount),
    messageParser(msParser)
{
    ui = std::make_unique<Ui::NewAccount>();
    ui->setupUi(this);
    connect(messageParser, SIGNAL(newAccountSignal(const QString&)), this, SLOT(newAccountSignalReceived(const QString&)));
}

NewAccount::~NewAccount()
{
}

void NewAccount::newAccountSignalReceived(const QString& status)
{
    if(status == "Success")
    {
        emit createNewAccountSuccess(ui->loginLine->text());
    }
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
    const QString login = ui->loginLine->text();
    const QString password1 = ui->passwordLine1->text();
    const QString password2 = ui->passwordLine2->text();

    if(password1 != password2)
    {
        ui->statusLabel->setText("Passwords don`t match");
        return;
    }
    messageParser->sendNewAccountMessage(login, password1);

}
