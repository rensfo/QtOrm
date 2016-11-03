#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T09:00:38
#
#-------------------------------------------------

include(QtOrmBuild.pri)

QT       += sql

QT       -= gui

TARGET = $$QTORM_LIB_NAME
TEMPLATE = lib

DEFINES += QTORM_LIBRARY
CONFIG += c++11

HEADERS += PropertyMap.h \
    ClassMap.h \
    ConfigurationMap.h \
    ClassMapBase.h \
    Exception.h \
    Session.h \
    SimpleSqlBuilder.h \
    SqlBuilderBase.h \
    OneToMany.h \
    OneToOne.h \
    Query.h \
    Relation.h \
    Operation.h \
    Condition.h \
    GroupConditions.h \
    Reestr.h \
    AutoUpdater.h

SOURCES += PropertyMap.cpp \
    ClassMap.cpp \
    ConfigurationMap.cpp \
    ClassMapBase.cpp \
    Exception.cpp \
    Session.cpp \
    SimpleSqlBuilder.cpp \
    SqlBuilderBase.cpp \
    OneToMany.cpp \
    OneToOne.cpp \
    Query.cpp \
    Relation.cpp \
    Condition.cpp \
    GroupConditions.cpp \
    Reestr.cpp \
    AutoUpdater.cpp

OTHER_FILES += QtOrmInclude.pri

qtormpri.files = QtOrmInclude.pri
qtormpri.path =  $$FRAMEWORK
INSTALLS += qtormpri \

headers.files = *.h
headers.path = $$QTORM_INSTALL_PATH
INSTALLS += headers

target.path = $$FRAMEWORK/libs
INSTALLS += target
