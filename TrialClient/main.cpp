#include <QApplication>
#include "client.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr("Fortune Client"));
    Dialog dialog;
    dialog.show();
    return app.exec();
}
