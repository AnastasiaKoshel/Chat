#include <QApplication>
#include "client.h"
#include "dialog.h"
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr("Client"));
    Login login;
    login.show();
    return app.exec();
}
