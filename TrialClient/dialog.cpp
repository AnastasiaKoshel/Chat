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
    /*
            QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Anastasiia_Koshel/SQlite/clientData.db");
    //qDebug() << QSqlDatabase::drivers();
    if (!db.open())
    {
       qDebug() << "Error: connection with database fail";
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM clientData");
    if(!query.exec())
    {
        qDebug() << "Can't Execute Query !";
    }
    else
    {
        qDebug() << "Query Executed Successfully !";
        while(query.next())
        {
            qDebug() << "Password : " << query.value(2).toString();
            qDebug() << "Login : " << query.value(1).toString();
            qDebug() << "Id : " << query.value(0).toString();
            client->contactsList.push_back(query.value(1).toString().toStdString());
        }
    }
    */
    for(auto user : usersArray)
    {
        ui->listWidget->addItem(user.toString());
        qDebug() << "Login received :" <<user.toString();
    }

    //client->requestAllUsers();

    connect(client, SIGNAL(processMessageSignal()), this, SLOT(displayMessage()));
    connect(client, SIGNAL(userIdbyLoginSignal(int, int)), this, SLOT(getChat(int, int)));
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
   // std::string currentChatLogin= item->text().toStdString();


    //QSqlQuery query;
    //query.prepare("SELECT * FROM clientData WHERE login = (:currentChatLogin)");
    //query.bindValue(":currentChatLogin", currentChatLogin.c_str());
    //query.first();
    client->setCurrentChatLogin(item->text().toStdString());
     qDebug()<<item->text();
   // displayChat();
}

void Dialog::getChat(int myId, int otherId)
{

}
