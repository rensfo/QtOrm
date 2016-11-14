#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T19:13:58
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_QueryModelsTestTest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

HEADERS += A.h \
    B.h \
    C.h \
    D.h \
    E.h \
    KindA.h \
    TypeA.h \
    AMap.h \
    BMap.h \
    CMap.h \
    DMap.h \
    EMap.h \
    KindAMap.h \
    TypeAMap.h \
    dml.h \
    Sql.h

SOURCES += tst_QueryModelsTestTest.cpp \
    A.cpp \
    B.cpp \
    C.cpp \
    D.cpp \
    E.cpp \
    KindA.cpp \
    TypeA.cpp

HEADERS += $$PWD/../../QtOrm/*.h
SOURCES += $$PWD/../../QtOrm/*.cpp

INCLUDEPATH += ../../QtOrm
DEPENDPATH += $$INCLUDEPATH

DEFINES += SRCDIR=\\\"$$PWD/\\\"
