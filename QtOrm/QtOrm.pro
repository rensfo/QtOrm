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

include(QtOrm.pri)

CONFIG += c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=


CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../libs/debug
} else {
    DESTDIR = $$PWD/../libs/release
}
