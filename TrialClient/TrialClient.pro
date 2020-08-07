QT += network widgets
requires(qtConfig(combobox))

HEADERS       = client.h \
    dialog.h
SOURCES       = client.cpp \
                dialog.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target

FORMS += \
    dialog.ui
