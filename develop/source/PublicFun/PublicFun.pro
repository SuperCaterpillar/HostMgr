#-------------------------------------------------
#
# Project created by QtCreator 2019-03-14T12:22:35
#
#-------------------------------------------------

QT       -= core gui

TARGET = PublicFun
TEMPLATE = lib

include(../../../HostMgr.pri)

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        PublicFun.cpp \
    Logging.cpp \
    StrategyBaseClass.cpp \
    Timer.cpp \
    HardWareInfo.cpp \
    SendDataToService.cpp \
    Tools.cpp \
    DateDispose.cpp \
    ProcessDispose.cpp

HEADERS += \
        PublicFun.h \
    Logging.h \
    Singleton.h \
    Publicdefine.h \
    StrategyBaseClass.h \
    kernelmacro.h \
    Timer.h \
    HardWareInfo.h \
    SendDataToService.h \
    Tools.h \
    DateDispose.h \
    ProcessDispose.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    ../../build/config/config.xml \
    ../../build/config/HsmModules.xml


#message($$OUT_PWD)
#Project MESSAGE: /data/build-HostMgr-Desktop_Qt_5_12_0_GCC_64bit-Debug/develop/source/PublicFun
