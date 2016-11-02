#-------------------------------------------------
#
# Project created by QtCreator 2016-10-31T11:11:27
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_SimpleSqlBuilderTestTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += A.h \
    TypeA.h \
    KindA.h \
    B.h \
    AMap.h \
    KindAMap.h \
    TypeAMap.h \
    Sql.h \
    SqlBuilderTest.h \
    BMap.h

SOURCES += tst_SimpleSqlBuilderTestTest.cpp \
    A.cpp \
    TypeA.cpp \
    KindA.cpp \
    B.cpp \
    SqlBuilderTest.cpp

HEADERS += $$PWD/../../QtOrm/*.h
SOURCES += $$PWD/../../QtOrm/*.cpp

INCLUDEPATH += ../../QtOrm
DEPENDPATH += $$INCLUDEPATH

DEFINES += SRCDIR=\\\"$$PWD/\\\"
