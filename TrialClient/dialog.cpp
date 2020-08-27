#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(Client *cl, QJsonArray usersArray, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    client(cl),
    usersList(usersArray)
{
    ui->setupUi(this);
    db = new MessagesDataBase();

    for(auto user : usersArray)
    {
        ui->listWidget->addItem(user.toString());
        qDebug() << "Login received :" <<user.toString();
    }


    connect(client, SIGNAL(processMessageSignal()), this, SLOT(displayMessage()));
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

void Dialog::displayMessage()
{
    ui->label->setText(client->messageCur.c_str());
}


void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug()<< "Clicked on list widget item";

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
    std::string fillerMessage;

    for(Message curMessage : messageHistory)
    {
        if(curMessage.isMyMessage)
        {
            myMessageText += (curMessage.text + '\n');
            qDebug()<<"My message "<<myMessageText.c_str();
            std::string fillerMessage(' ', curMessage.text.length());
            otherMessageText += (fillerMessage  + '\n');
        }
        else
        {
            otherMessageText += (curMessage.text  + '\n');
            std::string fillerMessage(' ', curMessage.text.length());
            myMessageText += (fillerMessage  + '\n');
        }

    }
    qDebug()<<"My messages "<<myMessageText.c_str();
    ui->labelYourMessage->setText(myMessageText.c_str());
    qDebug()<<"Other messages "<<myMessageText.c_str();
    ui->label->setText(otherMessageText.c_str());
}
