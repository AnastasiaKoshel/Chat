QT += network widgets \
    core sql

HEADERS       = dialog.h \
                dbmanager.h \
                messageparser.h \
                server.h \
                messagesdbmanager.h

SOURCES       = dialog.cpp \
                dbmanager.cpp \
                messageparser.cpp \
                server.cpp \
                main.cpp \
                messagesdbmanager.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneserver
INSTALLS += target

FORMS += \
    dialog.ui
