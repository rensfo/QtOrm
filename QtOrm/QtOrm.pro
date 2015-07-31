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

SOURCES += QtOrm.cpp

HEADERS += QtOrm.h\
        qtorm_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
