#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T09:00:38
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = QtOrm
TEMPLATE = lib

DEFINES += QTORM_LIBRARY

SOURCES += QtOrm.cpp \
    PropertyMap.cpp \
    ClassMap.cpp \
    ConfigurateMap.cpp \
    ClassMapBase.cpp \
    Exception.cpp \
    Session.cpp \
    SqlManagerBase.cpp \
    SimpleSqlManager.cpp

HEADERS += QtOrm.h\
        qtorm_global.h \
    PropertyMap.h \
    ClassMap.h \
    ConfigurateMap.h \
    ClassMapBase.h \
    Exception.h \
    Session.h \
    SqlManagerBase.h \
    Configurate.h \
    SimpleSqlManager.h \
    QtOrm

CONFIG += c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}
