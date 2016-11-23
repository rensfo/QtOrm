#include <QSqlError>
#include <QString>
#include <QtTest>

#include "A.h"
#include "AMap.h"
#include "BMap.h"
#include "CMap.h"
#include "DMap.h"
#include "EMap.h"
#include "Exception.h"
#include "KindAMap.h"
#include "Session.h"
#include "TypeAMap.h"
#include "dml.h"

using namespace QtOrm;
using namespace Sql;
using namespace Config;

class QueryResultTestTest : public QObject {
  Q_OBJECT

public:
  QueryResultTestTest();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void unregisteredClass();
  void objectFromReestr();
  void oneTableTwoTimesInQuery();
  void oneColumnTwoTimesInWhere();
  void insertObject();
  void deleteObject();
  void updateObject();
  void where();

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
  QSqlQuery query;
  QSqlDatabase db;
  QString dbName = "UnitTests.sqlite";
};

QueryResultTestTest::QueryResultTestTest() {
}

void QueryResultTestTest::initTestCase() {
  QVERIFY(openConnection());
  QVERIFY(createTables());
  QVERIFY(fillData());

  configurateSession();
}

void QueryResultTestTest::cleanupTestCase() {
  closeConnection();
  QVERIFY(dropDatabase(dbName));
}

void QueryResultTestTest::unregisteredClass() {
  try {
    auto res = session.getList<A>();
    Q_UNUSED(res)
  } catch (Exception ex) {
    QVERIFY(ex.getCode() == ErrorCode::NotRegistredClass);
    return;
  }
  QVERIFY(false);
}

void QueryResultTestTest::objectFromReestr() {
  try {
    registerClasses();
    A *a = session.getById<A>(1);
    A *aFromReestr = session.getById<A>(1);

    QVERIFY(a == aFromReestr);
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();

    QVERIFY(false);
  }
}

void QueryResultTestTest::oneTableTwoTimesInQuery() {
  try {
    //    connect(&session, &Session::executedSql, [](QString sql){ qDebug() << sql; });
    E *e = session.getById<E>(1);
    Q_UNUSED(e)

    QVERIFY(true);
  } catch (QtOrm::Exception &e) {
    Q_UNUSED(e)

    QVERIFY(false);
  }
}

void QueryResultTestTest::oneColumnTwoTimesInWhere()
{
  try {

    GroupConditions gc;
    gc.setOperation(GroupOperation::Or);
    gc.addConditionEqual("code_1", "code1");
    gc.addConditionEqual("code_1", "code2");

    QList<A *> *listA = session.getList<A>(gc);

    QCOMPARE(listA->count(), 2);
  } catch (QtOrm::Exception &e) {
    Q_UNUSED(e)

    QVERIFY(false);
  }
}

void QueryResultTestTest::insertObject() {
  try {
    A *a = new A();
    a->setCode("code10");
    session.saveObject(*a);

    session.clearReestr();

    a->deleteLater();

    a = nullptr;
    a = session.get<A>("code_1", "code10");

    a->deleteLater();

    QVERIFY(a);
  } catch (QtOrm::Exception &e) {
    qDebug().noquote() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::deleteObject() {
  try {
    A *a = session.getById<A>(1);
    session.deleteObject(*a);

    a->deleteLater();
    a = nullptr;

    a = session.getById<A>(1);

    QVERIFY(!a);

  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::updateObject()
{
  try {
    session.clearReestr();
    session.setAutoUpdate(false);

    A *a = session.getById<A>(3);

    a->setCode("x");
    session.saveObject(*a);

    session.clearReestr();

    a->deleteLater();
    a = nullptr;

    a = session.getById<A>(3);

    QCOMPARE(a->getCode(), QString("x"));

  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::where() {
  GroupConditions where;
  where.addConditionEqual("code_1", "code2");
  auto a = session.getList<A>(where);

  QCOMPARE(a->count(), 1);
}

bool QueryResultTestTest::openConnection() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbName);

  bool openResult = db.open();
  query = QSqlQuery(db);

  return openResult;
}

void QueryResultTestTest::configurateSession() {
  session.setDatabase(db);
}

void QueryResultTestTest::registerClasses() {
  ConfigurationMap::addMapping<AMap>();
  ConfigurationMap::addMapping<KindAMap>();
  ConfigurationMap::addMapping<TypeAMap>();
  ConfigurationMap::addMapping<BMap>();
  ConfigurationMap::addMapping<CMap>();
  ConfigurationMap::addMapping<DMap>();
  ConfigurationMap::addMapping<EMap>();
}

bool QueryResultTestTest::createTables() {
  return executeListCommand(sqlCreateSqlite);
}

bool QueryResultTestTest::fillData() {
  return executeListCommand(sqlFill);
}

bool QueryResultTestTest::executeListCommand(const QStringList &commands) {
  for (QString command : commands) {
    if (!query.exec(command)) {
      qDebug() << query.lastError().text();
      return false;
    }
  }

  return true;
}

bool QueryResultTestTest::dropDatabase(const QString &dbName) {

  return QFile::remove(dbName);
}

void QueryResultTestTest::closeConnection() {
  db.close();
}

QTEST_MAIN(QueryResultTestTest)

#include "tst_QueryResultTestTest.moc"
