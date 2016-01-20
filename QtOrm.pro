TEMPLATE = subdirs
SUBDIRS += \
    QtOrm \
    QtOrmTest

QtOrm.subdir        = QtOrm
QtOrmTest.subdir    = QtOrmTest

QtOrmTest.depends   = QtOrm
