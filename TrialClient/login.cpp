#include "login.h"
#include "ui_login.h"


Login::Login(MessageParser *msParser, QWidget *parent) :
    QDialog(parent),
    messageParser(msParser)
{
    ui = std::make_unique<Ui::Login>();
    ui->setupUi(this);
    connect(messageParser, SIGNAL(loginJsonSignal(const std::string&)), this, SLOT(loginSignalReceived(const std::string&)));
}

Login::~Login()
{
}

void Login::loginSignalReceived(const std::string& status)
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
    const std::string login = ui->loginLine->text().toStdString();
    const std::string password = ui->passwordLine->text().toStdString();

    messageParser->sendLoginMessage(login, password);

}

