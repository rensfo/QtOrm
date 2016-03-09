#-------------------------------------------------
#
# Project created by QtCreator 2016-02-05T12:32:33
#
#-------------------------------------------------

FRAMEWORK = $$(FRAMEWORK)
include(QtGridBuild.pri)

QT       -= gui
QT += widgets

TARGET = $$QTGRID_LIB_NAME
TEMPLATE = lib

DEFINES += QTGRID_LIBRARY
CONFIG += c++11

SOURCES += \
    Delegates/HeaderTableDelegate.cpp \
    Delegates/UniversalDelegate.cpp \
    Models/HeaderTableModel.cpp \
    Models/UniversalTableModel.cpp \
    Views/GridView.cpp \
    Views/HeaderTableView.cpp \
    Views/TableView.cpp \
    GridColumn.cpp \
    StaticFilter.cpp \
    StaticFilterItem.cpp

HEADERS += \
    Delegates/HeaderTableDelegate.h \
    Delegates/UniversalDelegate.h \
    Models/HeaderTableModel.h \
    Models/UniversalTableModel.h \
    Views/GridView.h \
    Views/HeaderTableView.h \
    Views/TableView.h \
    GridColumn.h \
    StaticFilter.h \
    StaticFilterItem.h

FORMS += \
    Views/GridView.ui

INCLUDEPATH += Models \
               Views \
               Delegates

OTHER_FILES += QtGridInclude.pri

qtgridpri.files = QtGridInclude.pri
qtgridpri.path =  $$FRAMEWORK
INSTALLS += qtgridpri \

headers.files = $$PWD/*.h
headers.path = $$QTGRID_INSTALL_PATH
INSTALLS += headers

headersdelegates.files = $$PWD/Delegates/*.h
headersdelegates.path = $$QTGRID_INSTALL_PATH/Delegates
INSTALLS += headersdelegates

headersmodels.files = $$PWD/Models/*.h
headersmodels.path = $$QTGRID_INSTALL_PATH/Models
INSTALLS += headersmodels

headersviews.files = $$PWD/Views/*.h
headersviews.path = $$QTGRID_INSTALL_PATH/Views
INSTALLS += headersviews

target.path = $$FRAMEWORK/libs
INSTALLS += target
