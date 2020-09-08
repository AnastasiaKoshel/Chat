#include "login.h"
#include "ui_login.h"


Login::Login(MessageParser *msParser, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login),
    messageParser(msParser)
{
    ui->setupUi(this);
    connect(messageParser, SIGNAL(loginJsonSignal(std::string)), this, SLOT(loginSignalReceived(std::string)));
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
        emit logInSuccess(ui->loginLine->text().toStdString());
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
    std::string login = ui->loginLine->text().toStdString();
    std::string password = ui->passwordLine->text().toStdString();

    messageParser->sendLoginMessage(login, password);

}

