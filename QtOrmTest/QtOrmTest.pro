#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T14:07:16
#
#-------------------------------------------------

QT       += testlib sql

QT       -= gui

TARGET = tst_qtormtesttest
CONFIG   += console c++11 testcase
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_qtormtesttest.cpp \
    TestClass.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../QtOrm/

CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../libs/debug/ -lQtOrm
} else {
    LIBS += -L$$PWD/../libs/release/ -lQtOrm
}

#win32:CONFIG(release, debug|release):
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/debug/ -lQtOrm
#else:unix:!macx: LIBS += -L$$PWD/../libs/debug/ -lQtOrm

INCLUDEPATH += $$PWD/../QtOrm
DEPENDPATH += $$PWD/../QtOrm

HEADERS += \
    TestClass.h
