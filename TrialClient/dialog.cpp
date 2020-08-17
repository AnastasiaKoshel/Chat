#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Client *cl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    client(cl)
{
    ui->setupUi(this);
    connect(client, SIGNAL(processMessageSignal()), this, SLOT(displayMessage()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_sendButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    client->sendTextMessage(message.toStdString());
    message = ui->labelYourMessage->text() + '\n'+ message;
    ui->labelYourMessage->setText(message);
    ui->textEdit->clear();
}

void Dialog::displayMessage()
{
    ui->label->setText(client->messageCur.c_str());
}

