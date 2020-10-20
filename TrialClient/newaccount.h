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
    void createNewAccountSuccess(const QString& login);

private slots:
    void newAccountSignalReceived(const QString& status);
    void on_okButton_clicked();

private:
    std::unique_ptr<Ui::NewAccount> ui;
    MessageParser *messageParser;

};

#endif // NEWACCOUNT_H
