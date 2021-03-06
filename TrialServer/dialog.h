#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "server.h"
namespace Ui {
class Dialog;

}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    Server server;



private slots:

    void on_quitButton_clicked();

    void on_connectButton_clicked();

    void on_sendMessage_clicked();

    void on_receiveButton_clicked();
    void displayMessage();

private:
    Ui::Dialog *ui;
};


#endif // DIALOG_H
