#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(MessageParser *msParser, QJsonArray usersArray, QString login, QWidget *parent) :
    QDialog(parent),
    messageParser(msParser),
    usersList(usersArray),
    login(login)
{
    ui = new Ui::Dialog();
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
    delete ui;
}

void Dialog::on_sendButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    messageParser->sendTextMessage(message, login, chatLogin);
    auto item = std::make_unique<QListWidgetItem>(message);
    item->setTextAlignment(Qt::AlignRight);
    ui->messagesWidget->addItem(item.get());
    ui->textEdit->clear();
}

void Dialog::displayMessage(const QString& message, const QString& senderLogin)
{
    qDebug()<<"curChatLogin "<<chatLogin<<" recipient Login "<<senderLogin;
    if(chatLogin == senderLogin)
    {
        auto item = std::make_unique<QListWidgetItem>(message);
        item->setTextAlignment(Qt::AlignLeft);
        ui->messagesWidget->addItem(item.get());
    }
}


void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug()<< "Clicked on list widget item and login is "<<item->text();

    chatLogin = item->text();
    messageParser->getSelectedChat(login, chatLogin);
    qDebug()<<item->text();

}

void Dialog::displayChat(const int myId, const int otherId)
{
    std::vector<Message> messageHistory = db->getMessageHistory(myId, otherId);
    QString myMessageText="";
    QString otherMessageText="";
    //const QString& image = "C:/Users/Anastasiia_Koshel/Documents/Chat/TrialClient/you.png";
    //QIcon icon("C:/Users/Anastasiia_Koshel/Documents/Chat/TrialClient/you.png");
    for(Message curMessage : messageHistory)
    {
        if(!curMessage.text.isEmpty())
        {
            QListWidgetItem* item = new QListWidgetItem(curMessage.text);
            if(curMessage.isMyMessage)
            {
                 item->setTextAlignment(Qt::AlignRight|Qt::AlignRight);
                 //item->setIcon(icon);
            }

            else
                item->setTextAlignment(Qt::AlignLeft);
            ui->messagesWidget->addItem(item);
        }

    }
}


