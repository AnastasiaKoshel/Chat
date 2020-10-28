QT += network widgets \
    core sql


#QMAKE_CXXFLAGS += /std:c++17

HEADERS       = client.h \
    dialog.h \
    filemanager.h \
    jsonType.h \
    login.h \
    mainwidget.h \
    messageparser.h \
    newaccount.h \
    messagesdatabase.h

SOURCES       = client.cpp \
                dialog.cpp \
                filemanager.cpp \
                login.cpp \
                main.cpp \
                mainwidget.cpp \
                messageparser.cpp \
                newaccount.cpp \
                messagesdatabase.cpp


FORMS += \
    dialog.ui \
    login.ui \
    mainWidget.ui \
    newAccount.ui
