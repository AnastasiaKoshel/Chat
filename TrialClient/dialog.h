#ifndef DIALOG_H
#define DIALOG_H


#include <string>
#include <QDialog>
#include "messageparser.h"
#include <QListWidgetItem>
#include "messagesdatabase.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(MessageParser *msParser, QJsonArray usersArray, std::string login, QWidget *parent = nullptr);
    ~Dialog();
    void displayChat();


private slots:

    void on_sendButton_clicked();
    void displayMessage(std::string message, std::string recipientLogin);
    void displayChat(int myId, int otherId);

    void on_listWidget_itemClicked(QListWidgetItem *item);
signals:
    void setCurChatLogin(std::string chatLogin);

private:
    Ui::Dialog *ui;
    MessageParser *messageParser;
    std::unique_ptr<MessagesDataBase> db;
    QJsonArray usersList;
    std::string login;
    std::string chatLogin;
};

#endif // DIALOG_H
