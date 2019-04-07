#-------------------------------------------------
#
# Project created by QtCreator 2019-04-07T11:06:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = KernelControl
TEMPLATE = lib


include(../../../HostMgr.pri)


SOURCES += \
        KernelControl.cpp \
    GenetlinkSocket.cpp

HEADERS += \
        KernelControl.h \
    GenetlinkSocket.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
