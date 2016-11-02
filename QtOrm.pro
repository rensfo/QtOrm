TEMPLATE = subdirs
SUBDIRS += \
    QtOrm \
    QtOrmTest

QtOrm.subdir        = QtOrm
QtOrmTest.subdir    = QtOrmTests

QtOrmTest.depends   = QtOrm
