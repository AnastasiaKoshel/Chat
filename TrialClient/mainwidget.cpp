#include "mainwidget.h"
#include "ui_mainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QDialog(parent),
    messageParser(new MessageParser())
{
    ui = std::make_unique<Ui::MainWidget>();
    login = new Login(messageParser);
    newAccount = new NewAccount(messageParser);

    ui->setupUi(this);
    connect(messageParser, SIGNAL(userListReceived(QJsonArray)), this, SLOT(showDialog(QJsonArray)));

    connect(login, SIGNAL(logInSuccess(const QString&)), this, SLOT(requestUserList(const QString&)));
    connect(newAccount, SIGNAL(createNewAccountSuccess(const QString&)), this, SLOT(requestUserList(const QString&)));

}

MainWidget::~MainWidget()
{
}


void MainWidget::on_mainLogInButton_clicked()
{
    this->close();
    login->show();
}

void MainWidget::requestUserList(const QString& log)
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
