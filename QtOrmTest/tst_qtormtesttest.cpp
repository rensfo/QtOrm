#include <QString>
#include <QtTest>

#include "Configurate.h"
#include "ClassMapBase.h"

#include "TestClass.h"

class QtOrmTestTest : public QObject {
  Q_OBJECT

public:
  QtOrmTestTest();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void ClassMapBaseTest();
};

QtOrmTestTest::QtOrmTestTest() {
}

void QtOrmTestTest::initTestCase() {
}

void QtOrmTestTest::ClassMapBaseTest() {
  using namespace QtOrm::Mapping;
  ClassMapBase *cmb = new ClassMapBase(this);

  QString tableName = "employee";
  QString idField = "id";
  QString idColumn = "idColumn";

  cmb->setMetaObject(TestClass::staticMetaObject);
  cmb->setTable(tableName);
  cmb->id(idField).setColumn(idColumn);


  QCOMPARE(cmb->getTable(), tableName);
  QCOMPARE(cmb->getIdProperty().getName(), idField);
  QCOMPARE(cmb->getIdProperty().getColumn(), idColumn);

}

void QtOrmTestTest::cleanupTestCase() {
}

QTEST_APPLESS_MAIN(QtOrmTestTest)

#include "tst_qtormtesttest.moc"
