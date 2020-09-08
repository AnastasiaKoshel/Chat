#include "mainwidget.h"
#include "ui_mainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWidget),
    messageParser(new MessageParser())
{
    login = new Login(messageParser);
    newAccount = new NewAccount(messageParser);

    ui->setupUi(this);
    connect(messageParser, SIGNAL(userListReceived(QJsonArray)), this, SLOT(showDialog(QJsonArray)));

    connect(login, SIGNAL(logInSuccess(std::string)), this, SLOT(requestUserList(std::string)));
    connect(newAccount, SIGNAL(createNewAccountSuccess(std::string)), this, SLOT(requestUserList(std::string)));

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
void MainWidget::requestUserList(std::string log)
{
    loginStr = log;
    messageParser->requestAllUsers();
}
void MainWidget::showDialog(const QJsonArray& userArray)
{
    dialog = new Dialog(messageParser, userArray, loginStr);
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
