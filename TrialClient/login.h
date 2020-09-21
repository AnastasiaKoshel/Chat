#ifndef LOGIN_H
#define LOGIN_H


#include <string>
#include <QDialog>
//#include "client.h"
#include "dialog.h"
#include "messageparser.h"


namespace Ui {
class Login;
}

class Login: public QDialog
{
    Q_OBJECT

public:
    explicit Login(MessageParser *msParser, QWidget *parent = nullptr);
    ~Login();

signals:
    void logInSuccess(const QString& login);

private slots:
    void loginSignalReceived(const QString& status);
    void on_loginButton_clicked();

private:
    std::unique_ptr<Ui::Login> ui;
    MessageParser *messageParser;

};

#endif // LOGIN_H
