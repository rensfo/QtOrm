#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T11:45:33
#
#-------------------------------------------------

ADVENT_DEV = $$(ADVENT_DEV)
include($$ADVENT_DEV/CommonInclude.pri)

QT       += sql testlib

QT       -= gui

TARGET = tst_BuilderBaseTest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app

SOURCES += tst_BuilderBaseTest.cpp \
    A.cpp \
    TypeA.cpp \
    KindA.cpp \
    B.cpp \
    SqlBuilderTest.cpp



DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    A.h \
    TypeA.h \
    KindA.h \
    B.h \
    AMap.h \
    KindAMap.h \
    TypeAMap.h \
    Sql.h \
    SqlBuilderTest.h \
    BMap.h

HEADERS += $$PWD/../../QtOrm/*.h
SOURCES += $$PWD/../../QtOrm/*.cpp

INCLUDEPATH += ../../QtOrm
DEPENDPATH += $$INCLUDEPATH
