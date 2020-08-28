#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "client.h"
#include <QtSQL>
#include <QListWidgetItem>
#include "messagesdatabase.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Client *cl, QJsonArray usersArray, QWidget *parent = nullptr);
    ~Dialog();
    void displayChat();


private slots:

    void on_sendButton_clicked();
    void displayMessage(std::string message, std::string recipientLogin);
    void displayChat(int myId, int otherId);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::Dialog *ui;
    Client* client;
    std::unique_ptr<MessagesDataBase> db;
    QJsonArray usersList;
};

#endif // DIALOG_H
