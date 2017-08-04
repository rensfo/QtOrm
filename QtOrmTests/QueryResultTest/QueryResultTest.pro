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
    $$QUERY_MODELS_TEST_PATH/EMap.h \
    $$QUERY_MODELS_TEST_PATH/SuperClassS.h \
    $$QUERY_MODELS_TEST_PATH/SubClassS1.h \
    $$QUERY_MODELS_TEST_PATH/SubClassS2.h \
    $$QUERY_MODELS_TEST_PATH/SubClassS3.h \
    $$QUERY_MODELS_TEST_PATH/SuperClassSMap.h \
    $$QUERY_MODELS_TEST_PATH/SubClassS1Map.h \
    $$QUERY_MODELS_TEST_PATH/SubClassS2Map.h \
    $$QUERY_MODELS_TEST_PATH/SubClassS3Map.h \
    AOnlyMap.h

SOURCES += tst_QueryResultTestTest.cpp \
    $$QUERY_MODELS_TEST_PATH/A.cpp \
    $$QUERY_MODELS_TEST_PATH/TypeA.cpp \
    $$QUERY_MODELS_TEST_PATH/KindA.cpp \
    $$QUERY_MODELS_TEST_PATH/B.cpp \
    $$QUERY_MODELS_TEST_PATH/D.cpp \
    $$QUERY_MODELS_TEST_PATH/E.cpp \
    $$QUERY_MODELS_TEST_PATH/C.cpp \
    $$QUERY_MODELS_TEST_PATH/SuperClassS.cpp \
    $$QUERY_MODELS_TEST_PATH/SubClassS1.cpp \
    $$QUERY_MODELS_TEST_PATH/SubClassS2.cpp \
    $$QUERY_MODELS_TEST_PATH/SubClassS3.cpp

QTORM_PATH = $$PWD/../../QtOrm
QTORM_QUERY_MODELS_PATH = $$QTORM_PATH/QueryModels
QTORM_RELATIONS_PATH = $$QTORM_PATH/Relations
QTORM_CONDITIONS_PATH = $$QTORM_PATH/Conditions

HEADERS += $$QTORM_PATH/*.h \
           $$QTORM_QUERY_MODELS_PATH/*.h \
           $$QTORM_RELATIONS_PATH/*.h \
           $$QTORM_CONDITIONS_PATH/*.h
SOURCES += $$QTORM_PATH/*.cpp \
           $$QTORM_QUERY_MODELS_PATH/*.cpp \
           $$QTORM_RELATIONS_PATH/*.cpp \
           $$QTORM_CONDITIONS_PATH/*.cpp

INCLUDEPATH += $$QTORM_PATH $$QTORM_QUERY_MODELS_PATH $$QTORM_RELATIONS_PATH $$QTORM_CONDITIONS_PATH $$QUERY_MODELS_TEST_PATH
DEPENDPATH += $$INCLUDEPATH

DEFINES += SRCDIR=\\\"$$PWD/\\\"
