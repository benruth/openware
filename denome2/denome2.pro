# Copyright (C) 2020 Benedikt Ruthenberg
# https://github.com/benruth
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_FILE = Denome2.rc

CONFIG += c++14

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
    SavedSettings.cpp \
    main.cpp \
    network/denonet/lib/ChannelVolumeCompleteInfo.cpp \
    network/denonet/lib/Denonet.cpp \
    ui/ChannelSlider.cpp \
    ui/HelpWizard/helpitem.cpp \
    ui/HelpWizard/helpwizard.cpp \
    ui/InfoDialog.cpp \
    ui_main.cpp

HEADERS += \
    SavedSettings.h \
    network/denonet/lib/ChannelVolumeCompleteInfo.h \
    network/denonet/lib/Denonet.h \
    network/denonet/lib/ToneSettings.h \
    network/denonet/lib/command_strings.h \
    network/denonet/lib/global.h \
    ui/ChannelSlider.h \
    ui/HelpWizard/helpitem.h \
    ui/HelpWizard/helpwizard.h \
    ui/InfoDialog.h \
    ui_main.h

FORMS += \
    ui/HelpWizard/helpwizard.ui \
    ui/InfoDialog.ui \
    ui_main.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    helpData.qrc \
    images.qrc \
    translations.qrc
