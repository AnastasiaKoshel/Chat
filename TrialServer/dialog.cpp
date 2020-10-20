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
    const bool serverInitSuccess = server.initServer();
    if(serverInitSuccess)
    {
        ui->label->setText("Server init succes");
        ui->connectButton->setEnabled(false);
    }
     const bool success = connect(&server, SIGNAL(processMessageSignal(QString)), this, SLOT(displayMessage(QString)));
     if(!success)
        ui->label->setText("failed connecting to displayMessage");

}


void Dialog::displayMessage(const QString& s)
{
     ui->label->setText(ui->label->text() + '\n' +s);
}
