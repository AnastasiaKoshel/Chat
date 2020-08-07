QT += network widgets

HEADERS       = dialog.h \
                server.h

SOURCES       = dialog.cpp \
                server.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneserver
INSTALLS += target

FORMS += \
    dialog.ui
