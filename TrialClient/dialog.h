#ifndef DIALOG_H
#define DIALOG_H


#include <string>
#include <QDialog>
#include "messageparser.h"
#include <QListWidgetItem>
#include "messagesdatabase.h"
#include <QFileDialog>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(MessageParser *msParser, QJsonArray usersArray, QString login, QWidget *parent = nullptr);
    ~Dialog();
    void displayChat();


private slots:

    void on_sendButton_clicked();
    void displayMessage(const QString& message, const QString& senderLogin);
    void displayChat(const int myId, const int otherId);
    void on_listWidget_itemClicked(QListWidgetItem *item);


    void on_uploadButton_clicked();

private:
    Ui::Dialog* ui;
    MessageParser *messageParser;
    std::unique_ptr<MessagesDataBase> db;
    QJsonArray usersList;
    QString login;
    QString chatLogin;
    bool isFileUploaded = false;
};

#endif // DIALOG_H
