#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "client.h"
#include <QtSQL>
#include <QListWidgetItem>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Client *cl, QWidget *parent = nullptr);
    ~Dialog();
    void displayChat();


private slots:

    void on_sendButton_clicked();
    void displayMessage();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::Dialog *ui;
    Client *client;
    QSqlDatabase db;
};

#endif // DIALOG_H
