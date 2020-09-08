QT += network widgets \
    core sql
requires(qtConfig(combobox))

QMAKE_CXXFLAGS += /std:c++17

HEADERS       = client.h \
    dialog.h \
    login.h \
    mainwidget.h \
    messageparser.h \
    newaccount.h \
    messagesdatabase.h
SOURCES       = client.cpp \
                dialog.cpp \
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
