#ifndef NEWACCOUNT_H
#define NEWACCOUNT_H

#include <string>
#include <QDialog>
#include "messageparser.h"
#include "dialog.h"


namespace Ui {
class NewAccount;
}

class NewAccount: public QDialog
{
    Q_OBJECT

public:
    explicit NewAccount(MessageParser *msParser, QWidget *parent = nullptr);
    ~NewAccount();


signals:
    void createNewAccountSuccess(std::string login);

private slots:
    void newAccountSignalReceived(std::string status);
    void on_okButton_clicked();

private:
    Ui::NewAccount *ui;
    MessageParser *messageParser;

};

#endif // NEWACCOUNT_H
