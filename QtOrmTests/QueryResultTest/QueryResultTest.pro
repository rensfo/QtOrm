#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T19:33:44
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_QueryResultTestTest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

QUERY_MODELS_TEST_PATH = $$PWD/../QueryModelsTest

HEADERS += \
    $$QUERY_MODELS_TEST_PATH/A.h \
    $$QUERY_MODELS_TEST_PATH/TypeA.h \
    $$QUERY_MODELS_TEST_PATH/KindA.h \
    $$QUERY_MODELS_TEST_PATH/B.h \
    $$QUERY_MODELS_TEST_PATH/AMap.h \
    $$QUERY_MODELS_TEST_PATH/KindAMap.h \
    $$QUERY_MODELS_TEST_PATH/TypeAMap.h \
    $$QUERY_MODELS_TEST_PATH/BMap.h \
    $$QUERY_MODELS_TEST_PATH/dml.h \
    $$QUERY_MODELS_TEST_PATH/C.h \
    $$QUERY_MODELS_TEST_PATH/CMap.h \
    $$QUERY_MODELS_TEST_PATH/D.h \
    $$QUERY_MODELS_TEST_PATH/E.h \
    $$QUERY_MODELS_TEST_PATH/DMap.h \
    $$QUERY_MODELS_TEST_PATH/EMap.h

SOURCES += tst_QueryResultTestTest.cpp \
    $$QUERY_MODELS_TEST_PATH/A.cpp \
    $$QUERY_MODELS_TEST_PATH/TypeA.cpp \
    $$QUERY_MODELS_TEST_PATH/KindA.cpp \
    $$QUERY_MODELS_TEST_PATH/B.cpp \
    $$QUERY_MODELS_TEST_PATH/D.cpp \
    $$QUERY_MODELS_TEST_PATH/E.cpp \
    $$QUERY_MODELS_TEST_PATH/C.cpp

QTORM_PATH = $$PWD/../../QtOrm

HEADERS += $$QTORM_PATH/*.h
SOURCES += $$QTORM_PATH/*.cpp

INCLUDEPATH += $$QTORM_PATH $$QUERY_MODELS_TEST_PATH
DEPENDPATH += $$INCLUDEPATH

DEFINES += SRCDIR=\\\"$$PWD/\\\"