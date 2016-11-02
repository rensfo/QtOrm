#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T11:45:33
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_BuilderBaseTest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE = app

SIMPLE_SQL_BUILDER_TEST_PATH = $$PWD/../SimpleSqlBuilderTest

SOURCES += tst_BuilderBaseTest.cpp \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/A.cpp \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/TypeA.cpp \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/KindA.cpp \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/B.cpp

HEADERS += \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/A.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/TypeA.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/KindA.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/B.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/AMap.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/KindAMap.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/TypeAMap.h \
    $$SIMPLE_SQL_BUILDER_TEST_PATH/BMap.h \
    dml.h

QTORM_PATH = $$PWD/../../QtOrm

HEADERS += $$QTORM_PATH/*.h
SOURCES += $$QTORM_PATH/*.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../QtOrm
DEPENDPATH += $$INCLUDEPATH
