QT += network widgets \
    core sql

HEADERS       = dialog.h \
                dbmanager.h \
                filemanager.h \
                jsonType.h \
                messageparser.h \
                server.h \
                messagesdbmanager.h

SOURCES       = dialog.cpp \
                dbmanager.cpp \
                filemanager.cpp \
                messageparser.cpp \
                server.cpp \
                main.cpp \
                messagesdbmanager.cpp

FORMS += \
    dialog.ui

CONFIG += release
