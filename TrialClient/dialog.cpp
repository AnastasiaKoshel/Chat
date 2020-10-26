#include "dialog.h"
#include "ui_dialog.h"
#include <QDesktopServices>

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

    connect(messageParser, SIGNAL(fileSavedSignal(const QString&)), this, SLOT(displayFile(const QString&)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_sendButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    if(!message.isEmpty())
        messageParser->sendTextMessage(message, login, chatLogin);

    QListWidgetItem* item = new QListWidgetItem(message);
    item->setTextAlignment(Qt::AlignRight);
    ui->messagesWidget->addItem(item);
    ui->textEdit->clear();
}

void Dialog::displayMessage(const QString& message, const QString& senderLogin)
{
    qDebug()<<"curChatLogin "<<chatLogin<<" recipient Login "<<senderLogin;
    if(chatLogin == senderLogin)
    {
        QListWidgetItem* item = new QListWidgetItem(message);
        item->setTextAlignment(Qt::AlignLeft);
        ui->messagesWidget->addItem(item);
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
    ui->messagesWidget->clear();
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

void Dialog::displayFile(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    QString fileName(fileInfo.fileName());
    QListWidgetItem* item = new QListWidgetItem(*fileIcon, fileName);
    item->setTextAlignment(Qt::AlignLeft);
    ui->messagesWidget->addItem(item);
    filesWidgetList.emplace(std::make_pair(item, filePath));
}

void Dialog::on_uploadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "open a file", "C:/");
    qDebug()<<"File path"<< filePath;
    messageParser->sendFileMessage(filePath, login, chatLogin);

    QFileInfo fileInfo(filePath);
    QString fileName(fileInfo.fileName());
    QListWidgetItem* item = new QListWidgetItem(*fileIcon, fileName);
    item->setTextAlignment(Qt::AlignRight);
    ui->messagesWidget->addItem(item);

    filesWidgetList.emplace(std::make_pair(item, filePath));
}

void Dialog::on_messagesWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto search = filesWidgetList.find(item);
    if(search == filesWidgetList.end())
        return;

    QDesktopServices::openUrl(QUrl::fromLocalFile(search->second));
}
