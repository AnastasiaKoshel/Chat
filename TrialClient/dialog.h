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
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    Client client;

private slots:

    void on_sendButton_clicked();

    void displayMessage();

    void on_connectButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
