#include "mainwidget.h"
#include "ui_mainWidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWidget),
    client(new Client())
{
    dialog = new Dialog(&client);
    login = new Login(&client);
    client.connectToServer();
    ui->setupUi(this);
    connect(login, SIGNAL(logInSuccess()), this, SLOT(showDialog()));
   // connect(&client, SIGNAL(processMessageSignal()), this, SLOT(displayMessage()));
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
    qDebug()<<"in Show Dialog";
    dialog->show();
    login->close();
}
