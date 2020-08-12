#include <QApplication>
#include "client.h"
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr("Client"));
    MainWidget mainWidget;
    mainWidget.show();
    return app.exec();
}
