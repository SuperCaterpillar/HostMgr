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

CONFIG += c++11

LIBS += -L$$DESTDIR/ -lPublicFun -lKernelControl

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

