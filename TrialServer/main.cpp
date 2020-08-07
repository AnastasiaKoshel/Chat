#include <QApplication>

#include "server.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Server::tr("Fortune Server"));
    Dialog dialog;
    dialog.show();
    return app.exec();
}
