#include "login.h"
#include "ui_login.h"


Login::Login(MessageParser *msParser, QWidget *parent) :
    QDialog(parent),
    messageParser(msParser)
{
    ui = std::make_unique<Ui::Login>();
    ui->setupUi(this);
    connect(messageParser, SIGNAL(loginJsonSignal(const QString&)), this, SLOT(loginSignalReceived(const QString&)));
}

Login::~Login()
{
}

void Login::loginSignalReceived(const QString& status)
{
    qDebug()<<"entered loginSignalReceived in Login class";
    if(status == "Success")
    {
        emit logInSuccess(ui->loginLine->text());
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
    const QString login = ui->loginLine->text();
    const QString password = ui->passwordLine->text();

    messageParser->sendLoginMessage(login, password);

}

