#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(MessageParser *msParser, QJsonArray usersArray, std::string login, QWidget *parent) :
    QDialog(parent),
    messageParser(msParser),
    usersList(usersArray),
    login(login)
{
    ui = std::make_unique<Ui::Dialog>();
    ui->setupUi(this);
    db = std::make_unique<MessagesDataBase>();

    ui->titleLabel->setText(login.c_str());
    for(auto user : usersArray)
    {
        ui->listWidget->addItem(user.toString());
        qDebug() << "Login received :" <<user.toString();
    }


    connect(messageParser, SIGNAL(processMessageSignal(const std::string&, const std::string&)),
            this, SLOT(displayMessage(const std::string&, const std::string&)));
    connect(messageParser, SIGNAL(userIdbyLoginSignal(const int, const int)), this, SLOT(displayChat(const int, const int)));
}

Dialog::~Dialog()
{
}

void Dialog::on_sendButton_clicked()
{
    std::string message = ui->textEdit->toPlainText().toStdString();
    messageParser->sendTextMessage(message, login, chatLogin);
    message = ui->labelYourMessage->text().toStdString() + '\n'+ message;
    ui->labelYourMessage->setText(message.c_str());
    ui->textEdit->clear();
}

void Dialog::displayMessage(const std::string& message, const std::string& senderLogin)
{
    qDebug()<<"curChatLogin "<<chatLogin.c_str()<<" recipient Login "<<senderLogin.c_str();
    if(chatLogin == senderLogin)
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

    chatLogin = item->text().toStdString();
    //client->setCurrentChatLogin();
    messageParser->getSelectedChat(login, chatLogin);
     qDebug()<<item->text();
   // displayChat();
}

void Dialog::displayChat(const int myId, const int otherId)
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
    //qDebug()<<"My messages "<<myMessageText;
    ui->labelYourMessage->setText(myMessageText.c_str());
    //qDebug()<<"Other messages "<<myMessageText;
    ui->label->setText(otherMessageText.c_str());
}
