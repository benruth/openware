QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_FILE = Denome2.rc

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    network/denonet/lib/ChannelVolumeCompleteInfo.cpp \
    network/denonet/lib/Denonet.cpp \
    ui_main.cpp

HEADERS += \
    network/denonet/lib/ChannelVolumeCompleteInfo.h \
    network/denonet/lib/Denonet.h \
    network/denonet/lib/ToneSettings.h \
    network/denonet/lib/command_strings.h \
    network/denonet/lib/global.h \
    ui_main.h

FORMS += \
    ui_main.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
