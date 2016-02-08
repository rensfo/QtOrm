#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T09:00:38
#
#-------------------------------------------------

FRAMEWORK = $$(FRAMEWORK)
include(QtOrmBuild.pri)
include($$FRAMEWORK/CommonInclude.pri)

QT       += sql

QT       -= gui

TARGET = $$QTORM_LIB_NAME
TEMPLATE = lib

DEFINES += QTORM_LIBRARY
CONFIG += c++11

HEADERS += $$PWD/PropertyMap.h \
    $$PWD/ClassMap.h \
    $$PWD/ConfigurateMap.h \
    $$PWD/ClassMapBase.h \
    $$PWD/Exception.h \
    $$PWD/Session.h \
    $$PWD/Configurate.h \
    $$PWD/QtOrm \
    $$PWD/SimpleSqlBuilder.h \
    $$PWD/SqlBuilderBase.h \
    $$PWD/OneToMany.h \
    $$PWD/OneToOne.h \
    $$PWD/FunctionSqlBuilder.h \
    $$PWD/PostgreFunctionSqlBuilder.h \

SOURCES += $$PWD/PropertyMap.cpp \
    $$PWD/ClassMap.cpp \
    $$PWD/ConfigurateMap.cpp \
    $$PWD/ClassMapBase.cpp \
    $$PWD/Exception.cpp \
    $$PWD/Session.cpp \
    $$PWD/SimpleSqlBuilder.cpp \
    $$PWD/SqlBuilderBase.cpp \
    $$PWD/OneToMany.cpp \
    $$PWD/OneToOne.cpp \
    $$PWD/FunctionSqlBuilder.cpp \
    $$PWD/PostgreFunctionSqlBuilder.cpp \

OTHER_FILES += QtOrmInclude.pri

qtormpri.files = QtOrmInclude.pri
qtormpri.path =  $$FRAMEWORK
INSTALLS += qtormpri \

headers.files = $$PWD/*.h
headers.path = $$QTORM_INSTALL_PATH
INSTALLS += headers

target.path = $$FRAMEWORK/libs
INSTALLS += target
