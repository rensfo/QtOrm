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

HEADERS += Exception.h \
    Session.h \
    SimpleSqlBuilder.h \
    SqlBuilderBase.h \
    Query.h \
    GroupConditions.h \
    AutoUpdater.h \
    QueryTableModel.h \
    QueryJoin.h \
    Registry.h \
    Mappings/PropertyMap.h \
    Mappings/ClassMap.h \
    Mappings/ConfigurationMap.h \
    Mappings/ClassMapBase.h \
    Mappings/SubClassMap.h \
    Mappings/ClassMapBaseExt.h \
    Mappings/SingleTableInheritanceMap.h \
    Mappings/ClassTableInheritanceMap.h \
    Relations/Relation.h \
    Relations/OneToMany.h \
    Relations/OneToOne.h \
    QueryModels/QueryModel.h \
    QueryModels/SelectQueryModel.h \
    QueryModels/InsertQueryModel.h \
    QueryModels/DeleteQueryModel.h \
    QueryModels/UpdateQueryModel.h \
    QueryModels/UpdateFieldQueryModel.h \
    QueryModels/OrderColumn.h \
    Conditions/Operation.h \
    Conditions/Condition.h

SOURCES += Exception.cpp \
    Session.cpp \
    SimpleSqlBuilder.cpp \
    SqlBuilderBase.cpp \
    Query.cpp \
    GroupConditions.cpp \
    AutoUpdater.cpp \
    QueryTableModel.cpp \
    QueryJoin.cpp \
    Registry.cpp \
    Mappings/PropertyMap.cpp \
    Mappings/ConfigurationMap.cpp \
    Mappings/ClassMapBase.cpp \
    Mappings/SubClassMap.cpp \
    Relations/Relation.cpp \
    Relations/OneToMany.cpp \
    Relations/OneToOne.cpp \
    QueryModels/QueryModel.cpp \
    QueryModels/SelectQueryModel.cpp \
    QueryModels/InsertQueryModel.cpp \
    QueryModels/DeleteQueryModel.cpp \
    QueryModels/UpdateQueryModel.cpp \
    QueryModels/UpdateFieldQueryModel.cpp \
    Conditions/Condition.cpp

OTHER_FILES += QtOrmInclude.pri

qtormpri.files = QtOrmInclude.pri
qtormpri.path =  $$INSTALL_PATH
INSTALLS += qtormpri \

headers.files = *.h
headers.path = $$QTORM_INSTALL_PATH
INSTALLS += headers

target.path = $$INSTALL_PATH/lib
INSTALLS += target
