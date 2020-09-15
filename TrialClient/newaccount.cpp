#include "newaccount.h"
#include "ui_newAccount.h"

NewAccount::NewAccount(MessageParser *msParser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAccount),
    messageParser(msParser)
{
    ui = std::make_unique<Ui::NewAccount>();
    ui->setupUi(this);
    connect(messageParser, SIGNAL(newAccountSignal(const std::string&)), this, SLOT(newAccountSignalReceived(const std::string&)));
}

NewAccount::~NewAccount()
{
}

void NewAccount::newAccountSignalReceived(const std::string& status)
{
    if(status == "Success")
    {
        emit createNewAccountSuccess(ui->loginLine->text().toStdString());
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
    const std::string login = ui->loginLine->text().toStdString();
    const std::string password1 = ui->passwordLine1->text().toStdString();
    const std::string password2 = ui->passwordLine2->text().toStdString();

    if(password1 != password2)
    {
        ui->statusLabel->setText("Passwords don`t match");
        return;
    }
    messageParser->sendNewAccountMessage(login, password1);

}
