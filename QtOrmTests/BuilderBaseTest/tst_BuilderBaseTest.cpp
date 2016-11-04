#include <QCoreApplication>
#include <QString>
#include <QtTest>

#include "A.h"
#include "AMap.h"
#include "BMap.h"
#include "Exception.h"
#include "KindAMap.h"
#include "Session.h"
#include "SqlBuilderTest.h"
#include "TypeAMap.h"
#include "dml.h"

using namespace QtOrm;
using namespace Sql;
using namespace Config;

class BuilderBaseTest : public QObject {
  Q_OBJECT

public:
  explicit BuilderBaseTest(QObject *parent = nullptr);

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void unregisteredClass();
  void objectFromReestr();

private:
  bool openConnection();
  void configurateSession();
  void registerClasses();
  bool createTables();
  bool fillData();
  bool executeListCommand(const QStringList &commands);

  bool dropDatabase(const QString &dbName);
  void closeConnection();

private:
  QtOrm::Session session;
  QSqlDatabase db;
  QSqlQuery query;
  QString dbName = "UnitTests.sqlite";
};

BuilderBaseTest::BuilderBaseTest(QObject *parent) : QObject(parent) {
}

void BuilderBaseTest::initTestCase() {
  QVERIFY(openConnection());
  QVERIFY(createTables());
  QVERIFY(fillData());

  configurateSession();
}

bool BuilderBaseTest::openConnection() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbName);

  bool openResult = db.open();
  query = QSqlQuery(db);

  return openResult;
}

bool BuilderBaseTest::createTables() {
  return executeListCommand(sqlCreateSqlite);
}

bool BuilderBaseTest::fillData() {
  return executeListCommand(sqlFill);
}

bool BuilderBaseTest::executeListCommand(const QStringList &commands) {
  for (QString command : commands) {
    if (!query.exec(command)) {
      qDebug() << query.lastError().text();
      return false;
    }
  }

  return true;
}

void BuilderBaseTest::configurateSession() {
  session.setDatabase(db);
}

void BuilderBaseTest::registerClasses() {
  ConfigurationMap::addMapping<AMap>();
  ConfigurationMap::addMapping<KindAMap>();
  ConfigurationMap::addMapping<TypeAMap>();
  ConfigurationMap::addMapping<BMap>();
}

void BuilderBaseTest::cleanupTestCase() {
  QVERIFY(dropDatabase(dbName));
  closeConnection();
}

bool BuilderBaseTest::dropDatabase(const QString &dbName) {
  return QFile::remove(dbName);
}

void BuilderBaseTest::closeConnection() {
  db.close();
}

void BuilderBaseTest::unregisteredClass() {
  try {
    auto res = session.getList<A>();
    Q_UNUSED(res)
  } catch (Exception ex) {
    QVERIFY(ex.getCode() == ErrorCode::NotRegistredClass);
    return;
  }
  QVERIFY(false);
}

void BuilderBaseTest::objectFromReestr() {
  registerClasses();
  A *a = session.getById<A>(1);
  A *aFromReestr = session.getById<A>(1);

  QVERIFY(a == aFromReestr);
}

QTEST_MAIN(BuilderBaseTest)

#include "tst_BuilderBaseTest.moc"
