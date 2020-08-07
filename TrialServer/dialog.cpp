#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_quitButton_clicked()
{
    this->close();
}

void Dialog::on_connectButton_clicked()
{
    bool serverInitSuccess = server.initServer();
    if(serverInitSuccess)
        ui->label->setText("Server init succes");
     bool success = connect(&server, SIGNAL(processMessageSignal()), this, SLOT(displayMessage()));
     if(!success)
ui->label->setText("Faaaail");

}

void Dialog::on_sendMessage_clicked()
{
    QString message = ui->textEdit->toPlainText();
    server.sendMessage(message.toStdString());
}


void Dialog::on_receiveButton_clicked()
{
    ui->label->setText(server.messageCur.data());
}
void Dialog::displayMessage()
{
     ui->label->setText(server.messageCur.data());
}
