include($$FRAMEWORK/CommonInclude.pri)
QTORM_LIB_NAME = QtOrm$${QT_MAJOR_VERSION}$${LIB_SUFFIX}
QTORM_FOLDER_NAME = QtOrm
QTORM_INCLUDE = $$FRAMEWORK_INCLUDE/$$QTORM_FOLDER_NAME
QTORM_HEADERS = $$QTORM_INCLUDE/*.h

INCLUDEPATH += $$QTORM_INCLUDE
DEPENDPATH += $$INCLUDEPATH $$FRAMEWORK_INSTALL_LIBS

!contains(HEADERS, $$QTORM_HEADERS) HEADERS += $$QTORM_HEADERS

LIBS += -L$$FRAMEWORK_INSTALL_LIBS -l$$QTORM_LIB_NAME
