#ifndef NEWACCOUNT_H
#define NEWACCOUNT_H

#include <QDialog>
#include "client.h"
#include "dialog.h"

namespace Ui {
class NewAccount;
}

class NewAccount: public QDialog
{
    Q_OBJECT

public:
    explicit NewAccount(Client *cl, QWidget *parent = nullptr);
    ~NewAccount();


signals:
    void createNewAccountSuccess();

private slots:
    void newAccountSignalReceived(std::string status);
    void on_okButton_clicked();

private:
    Ui::NewAccount *ui;
    Client *client;

};

#endif // NEWACCOUNT_H
