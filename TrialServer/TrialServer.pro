QT += network widgets \
    core sql

HEADERS       = dialog.h \
                dbmanager.h \
                server.h

SOURCES       = dialog.cpp \
                dbmanager.cpp \
                server.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneserver
INSTALLS += target

FORMS += \
    dialog.ui
