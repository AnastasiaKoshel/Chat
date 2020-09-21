#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(MessageParser *msParser, QJsonArray usersArray, QString login, QWidget *parent) :
    QDialog(parent),
    messageParser(msParser),
    usersList(usersArray),
    login(login)
{
    ui = std::make_unique<Ui::Dialog>();
    ui->setupUi(this);
    db = std::make_unique<MessagesDataBase>();

    ui->titleLabel->setText(login);
    for(auto user : usersArray)
    {
        ui->listWidget->addItem(user.toString());
        qDebug() << "Login received :" <<user.toString();
    }


    connect(messageParser, SIGNAL(processMessageSignal(const QString&, const QString&)),
            this, SLOT(displayMessage(const QString&, const QString&)));
    connect(messageParser, SIGNAL(userIdbyLoginSignal(const int, const int)), this, SLOT(displayChat(const int, const int)));
}

Dialog::~Dialog()
{
}

void Dialog::on_sendButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    messageParser->sendTextMessage(message, login, chatLogin);
    message = ui->labelYourMessage->text() + '\n'+ message;
    ui->labelYourMessage->setText(message);
    ui->textEdit->clear();
}

void Dialog::displayMessage(const QString& message, const QString& senderLogin)
{
    qDebug()<<"curChatLogin "<<chatLogin<<" recipient Login "<<senderLogin;
    if(chatLogin == senderLogin)
    {
        QString curMessage = ui->label->text();
        curMessage += "\n";
        curMessage += message;
        ui->label->setText(curMessage);
    }
}


void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug()<< "Clicked on list widget item and login is "<<item->text();

    chatLogin = item->text();
    //client->setCurrentChatLogin();
    messageParser->getSelectedChat(login, chatLogin);
     qDebug()<<item->text();
   // displayChat();
}

void Dialog::displayChat(const int myId, const int otherId)
{
    std::vector<Message> messageHistory = db->getMessageHistory(myId, otherId);
    QString myMessageText="";
    QString otherMessageText="";

    for(Message curMessage : messageHistory)
    {
        if(curMessage.isMyMessage && !curMessage.text.isEmpty())
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
    //qDebug()<<"My messages "<<myMessageText;
    ui->labelYourMessage->setText(myMessageText);
    //qDebug()<<"Other messages "<<myMessageText;
    ui->label->setText(otherMessageText);
}
