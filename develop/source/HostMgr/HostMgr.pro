#-------------------------------------------------
#
# Project created by QtCreator 2019-03-13T23:04:25
#
#-------------------------------------------------

QT       -= core gui



TARGET = HostMgr
CONFIG += c++11 console
CONFIG -= app_bundle

include(../../../HostMgr.pri)

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11



SOURCES += \
        main.cpp \
    LoadSubModules.cpp

HEADERS += \
    LoadSubModules.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#unix:!macx: LIBS += -L$$DESTDIR/ -lBaseEnv -lPublicFun
#message($$OUT_PWD)

