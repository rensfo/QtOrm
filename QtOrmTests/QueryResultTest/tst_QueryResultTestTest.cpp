﻿#include <QSqlError>
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
  void objectFromRegistry();
  void oneTableTwoTimesInQuery();
  void oneColumnTwoTimesInWhere();
  void insertObject();
  void deleteObject();
  void updateObject();
  void where();
  void refreshObject();
  void refreshChildObject();
  void deleteChildAndRefresh();
  void childrenOneToOneParent();
  void autoUpdate();
  void operationBetween();
  void operationIn();
  void operationGreater();
  void operationGreaterOrEqual();
  void operationLess();
  void operationLessOrEqual();
  void operationLike();

private:
  bool openConnection();
  void configurateSession();
  void registerClasses();
  bool createTables();
  bool fillData();
  bool executeListCommand(const QStringList &commands);

  bool dropDatabase(const QString &dbName);
  void closeConnection();

  template <typename T>
  T find(QList<T> container, std::function<bool(T)> func);

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
  } catch (NotRegistredClassException &e) {
    Q_UNUSED(e)
    QVERIFY(true);
    return;
  }
  QVERIFY(false);
}

void QueryResultTestTest::objectFromRegistry() {
  try {
    registerClasses();
    QSharedPointer<A> a = session.getById<A>(1);
    QSharedPointer<A> aFromRegistry = session.getById<A>(1);

    QVERIFY(a == aFromRegistry);
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();

    QVERIFY(false);
  }
}

void QueryResultTestTest::oneTableTwoTimesInQuery() {
  try {
    QSharedPointer<E> e = session.getById<E>(1);
    Q_UNUSED(e)

    QVERIFY(true);
  } catch (QtOrm::Exception &e) {
    Q_UNUSED(e)

    QVERIFY(false);
  }
}

void QueryResultTestTest::oneColumnTwoTimesInWhere() {
  try {

    GroupConditions gc;
    gc.setOperation(GroupOperation::Or);
    gc.addEqual("code_1", "code1");
    gc.addEqual("code_1", "code2");

    QList<QSharedPointer<A>> listA = session.getList<A>(gc);

    QCOMPARE(listA.count(), 2);
  } catch (QtOrm::Exception &e) {
    Q_UNUSED(e)

    QVERIFY(false);
  }
}

void QueryResultTestTest::insertObject() {
  try {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode("code10");
    session.saveObject(a);

    session.clearRegistry();

    a->deleteLater();

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
    QSharedPointer<A> a = session.getById<A>(1);
    session.deleteObject<A>(a);

    a.clear();
    a = session.getById<A>(1);

    QVERIFY(!a);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::updateObject() {
  try {
    session.clearRegistry();
    session.setAutoUpdate(false);

    QSharedPointer<A> a = session.getById<A>(3);

    a->setCode("x");
    session.saveObject(a);

    session.clearRegistry();

    a->deleteLater();

    a = session.getById<A>(3);

    QCOMPARE(a->getCode(), QString("x"));

  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::where() {
  GroupConditions where;
  where.addEqual("code_1", "code2");
  auto a = session.getList<A>(where);

  QCOMPARE(a.count(), 1);
}

void QueryResultTestTest::refreshObject() {
  try {
    QSqlQuery query(db);

    QSharedPointer<A> a = session.get<A>("code_1", "code2");
    if (query.exec("update A set code = null where code = 'code2'")) {
      session.refresh<A>(a);
      QCOMPARE(a->getCode(), QString(""));

      a->setCode("code2");
      session.saveObject(a);
      return;
    }
    QVERIFY(false);
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::refreshChildObject() {
  try {
    QSqlQuery query(db);

    QSharedPointer<A> a = session.get<A>("code_1", "code2");
    if (query.exec("update B set code = 'code2.2.1' where code = 'code2.2'")) {
      session.refresh(a);

      std::function<bool(QSharedPointer<B>)> func =
          [](QSharedPointer<B> item) { return item->getCode() == "code2.2.1"; };
      QSharedPointer<B> updatedB = this->find<QSharedPointer<B>>(a->getChild(), func);
      QVERIFY(updatedB != nullptr);
      if (updatedB) {
        updatedB->setCode("code2.2");
        session.saveObject(updatedB);
      }
      return;
    }
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }

  QVERIFY(false);
}

void QueryResultTestTest::deleteChildAndRefresh() {
  try {

    QSqlQuery query(db);

    QSharedPointer<A> a = session.get<A>("code_1", "code2");
    if (query.exec("delete from B where code = 'code2.2'")) {
      session.refresh(a);
      QCOMPARE(a->getChild().count(), 2);
      return;
    }
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }

  QVERIFY(false);
}

void QueryResultTestTest::childrenOneToOneParent() {
  try {
    QSharedPointer<A> a = session.get<A>("code_1", "code2");

    QCOMPARE(a, a->getChild().first()->getA());
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::autoUpdate() {
  try {
    session.clearRegistry();
    session.clearQueryCache();
    session.setAutoUpdate(true);

    QString newCode = "code2.2";
    QString oldCode = "code2";

    auto a = session.get<A>("code_1", oldCode);
    a->setCode(newCode);

    session.clearRegistry();
    a = session.get<A>("code_1", newCode);
    QCOMPARE(a->getCode(), newCode);

    a->setCode(oldCode);
    session.setAutoUpdate(false);
    session.clearRegistry();
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationBetween() {
  try {
//        connect(&session, &Session::executedSql, [](QString sql){ qDebug() << sql; });
    GroupConditions where;
    where.addBetween("id", 2, 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationIn() {
  try {
    //    connect(&session, &Session::executedSql, [](QString sql){ qDebug() << sql; });
    GroupConditions where;
    where.addIn("id", {3, 4});
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationGreater() {
  try {
    //    connect(&session, &Session::executedSql, [](QString sql){ qDebug() << sql; });
    GroupConditions where;
    where.addGreater("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 1);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationGreaterOrEqual() {
  try {
    GroupConditions where;
    where.addGreaterOrEqual("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationLess() {
  try {
    GroupConditions where;
    where.addLess("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 1);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationLessOrEqual() {
  try {
    GroupConditions where;
    where.addLessOrEqual("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationLike() {
  try {
    GroupConditions where;
    where.addLike("code_1", "code");
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

bool QueryResultTestTest::openConnection() {
  dropDatabase(dbName);

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
  ConfigurationMap::addMappings<AMap, KindAMap, TypeAMap, BMap, CMap, DMap, EMap>();
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

template <typename T>
T QueryResultTestTest::find(QList<T> container, std::function<bool(T)> func) {
  for (T element : container) {
    if (func(element)) {
      return element;
    }
  }
  return T();
}

QTEST_MAIN(QueryResultTestTest)

#include "tst_QueryResultTestTest.moc"
