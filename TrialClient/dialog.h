#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "client.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Client *cl, QWidget *parent = nullptr);
    ~Dialog();


private slots:

    void on_sendButton_clicked();
    void displayMessage();

private:
    Ui::Dialog *ui;
    Client *client;
};

#endif // DIALOG_H
