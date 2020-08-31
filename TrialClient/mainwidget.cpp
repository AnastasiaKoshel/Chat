#include "mainwidget.h"
#include "ui_mainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWidget),
    client(new Client())
{
    login = new Login(client);
    newAccount = new NewAccount(client);
    client->connectToServer();
    ui->setupUi(this);
    connect(client, SIGNAL(userListReceived(QJsonArray)), this, SLOT(showDialog(QJsonArray)));
    connect(login, SIGNAL(logInSuccess()), this, SLOT(requestUserList()));
    connect(newAccount, SIGNAL(createNewAccountSuccess()), this, SLOT(requestUserList()));
}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::on_mainLogInButton_clicked()
{
    this->close();
    login->show();
}
void MainWidget::requestUserList()
{
    client->requestAllUsers();
}
void MainWidget::showDialog(const QJsonArray& userArray)
{
    dialog = new Dialog(client, userArray);
    qDebug()<<"Entered Show Dialog";
    dialog->show();
    login->close();
    newAccount->close();
}

void MainWidget::on_newAccountButton_clicked()
{
    this->close();
    newAccount->show();
}
