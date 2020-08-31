QT += network widgets\
    core sql
requires(qtConfig(combobox))

HEADERS       = client.h \
    dialog.h \
    login.h \
    mainwidget.h \
    newaccount.h
SOURCES       = client.cpp \
                dialog.cpp \
                login.cpp \
                main.cpp \
                mainwidget.cpp \
                newaccount.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS += \
    dialog.ui \
    login.ui \
    mainWidget.ui \
    newAccount.ui
