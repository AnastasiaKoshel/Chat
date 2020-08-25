#include "mainwidget.h"
#include "ui_mainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWidget),
    client(new Client())
{
    dialog = new Dialog(&client);
    login = new Login(&client);
    newAccount = new NewAccount(&client);
    client.connectToServer();
    ui->setupUi(this);
    connect(login, SIGNAL(logInSuccess()), this, SLOT(showDialog()));
    connect(newAccount, SIGNAL(createNewAccountSuccess()), this, SLOT(showDialog()));
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

void MainWidget::showDialog()
{
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
