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

QTORM_PATH = $$PWD/../../QtOrm
QTORM_QUERY_MODELS_PATH = $$QTORM_PATH/QueryModels
QTORM_RELATIONS_PATH = $$QTORM_PATH/Relations

HEADERS += $$QTORM_PATH/*.h \
           $$QTORM_QUERY_MODELS_PATH/*.h \
           $$QTORM_RELATIONS_PATH/*.h
SOURCES += $$QTORM_PATH/*.cpp \
           $$QTORM_QUERY_MODELS_PATH/*.cpp \
           $$QTORM_RELATIONS_PATH/*.cpp

INCLUDEPATH += $$QTORM_PATH $$QTORM_QUERY_MODELS_PATH $$QTORM_RELATIONS_PATH
DEPENDPATH += $$INCLUDEPATH

DEFINES += SRCDIR=\\\"$$PWD/\\\"
