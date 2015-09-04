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
    ConfigurateMap.cpp

HEADERS += QtOrm.h\
        qtorm_global.h \
    PropertyMap.h \
    ClassMap.h \
    ConfigurateMap.h \
    configurate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
