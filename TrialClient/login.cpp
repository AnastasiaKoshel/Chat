#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login),
    client(new Client())
{
    ui->setupUi(this);
    connect(&client, SIGNAL(processMessageSignal()), this, SLOT(displayMessage()));
}

Login::~Login()
{
    delete ui;
}


void Login::on_loginButton_clicked()
{
    QString login = ui->loginLine->text();
    QString password = ui->loginLine->text();
    QString messageForServer = "Login:"+login+"Password:"+password;
    client.sendMessage(messageForServer.toStdString());
}
