#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Client *cl, QJsonArray usersArray, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    client(cl),
    usersList(usersArray)
{
    ui->setupUi(this);
    db = std::make_unique<MessagesDataBase>();

    ui->titleLabel->setText(cl->getLogin().c_str());
    for(auto user : usersArray)
    {
        ui->listWidget->addItem(user.toString());
        qDebug() << "Login received :" <<user.toString();
    }


    connect(client, SIGNAL(processMessageSignal(std::string, std::string)), this, SLOT(displayMessage(std::string, std::string)));
    connect(client, SIGNAL(userIdbyLoginSignal(int, int)), this, SLOT(displayChat(int, int)));
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

void Dialog::displayMessage(std::string message, std::string recipientLogin)
{
    qDebug()<<"curChatLogin "<<client->getLogin().c_str()<<" recipient Login "<<recipientLogin.c_str();
    if(client->getLogin() == recipientLogin)
    {
        std::string curMessage = ui->label->text().toStdString();
        curMessage += "\n";
        curMessage += message;
        ui->label->setText(curMessage.c_str());
    }
}


void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug()<< "Clicked on list widget item and login is "<<item->text();

    client->setCurrentChatLogin(item->text().toStdString());
    client->getSelectedChat();
     qDebug()<<item->text();
   // displayChat();
}

void Dialog::displayChat(int myId, int otherId)
{
    std::vector<Message> messageHistory = db->getMessageHistory(myId, otherId);
    std::string myMessageText="";
    std::string otherMessageText="";

    for(Message curMessage : messageHistory)
    {
        if(curMessage.isMyMessage && !curMessage.text.empty())
        {
            myMessageText += (curMessage.text + "\n");
            otherMessageText += "\n";
        }
        else
        {
            otherMessageText += (curMessage.text  + "\n");
            myMessageText += "\n";
        }

    }
    qDebug()<<"My messages "<<myMessageText.c_str();
    ui->labelYourMessage->setText(myMessageText.c_str());
    qDebug()<<"Other messages "<<myMessageText.c_str();
    ui->label->setText(otherMessageText.c_str());
}
