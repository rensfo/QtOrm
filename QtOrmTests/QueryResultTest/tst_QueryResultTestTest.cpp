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
#include "SubClassS1Map.h"
#include "SubClassS2Map.h"
#include "SubClassS3Map.h"
#include "SuperClassSMap.h"
#include "TypeAMap.h"
#include "dml.h"
#include "Conditions/Condition.h"

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
  void orderBy();
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
  void nullValueUpdate();
  void OneTablePerHierarchySelect();
  void OneTablePerHierarchyConcreteSelect();
  void OneTablePerHierarchyInsert();
  void TablePerHierarchySelectWithReference();

private:
  void enableLogSql();
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

    a = session.get<A>("code_1", "code10");

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

void QueryResultTestTest::orderBy() {
  try {
    //            connect(&session, &Session::executedSql, [](QString sql){ qDebug() << sql; });
    session.clearRegistry();
    QSharedPointer<A> a = session.getById<A>(2);

    QCOMPARE(a->getChild()[0]->getCode(), QString("code2.3"));
    QCOMPARE(a->getChild()[1]->getCode(), QString("code2.2"));
    QCOMPARE(a->getChild()[2]->getCode(), QString("code2.1"));

  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
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

      std::function<bool(QSharedPointer<B>)> func = [](QSharedPointer<B> item) {
        return item->getCode() == "code2.2.1";
      };
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

    QCOMPARE(a, a->getChild().first()->getA().toStrongRef());
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::autoUpdate() {
  try {
    session.clearRegistry();
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

void QueryResultTestTest::nullValueUpdate() {
  try {
    auto sub = session.getById<SubClassS1>(1);
    sub->setIntVal(0);
    session.saveObject(sub);
    if (!query.exec("select int_val from super_class_s where id = 1")) {
      qDebug() << query.lastError().text();
      QVERIFY(false);
      return;
    }

    query.next();
    QCOMPARE(query.value("int_val").isNull(), true);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::OneTablePerHierarchySelect() {
  try {
    auto supers = session.getList<SuperClassS>();

    for (auto s : supers) {
      bool result = true;
      QList<long> SubClassS1Ids{1, 2};
      QList<long> SubClassS2Ids{3, 4};
      if (SubClassS1Ids.contains(s->getId())) {
        result = qobject_cast<SubClassS1 *>(s);
      } else if (SubClassS2Ids.contains(s->getId())) {
        result = qobject_cast<SubClassS2 *>(s);
      } else {
        result = qobject_cast<SubClassS3 *>(s);
      }

      if (!result) {
        QVERIFY(false);
        return;
      }
    }
    QVERIFY(true);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::OneTablePerHierarchyConcreteSelect() {
  try {
    auto subs = session.getList<SubClassS1>();
    QCOMPARE(subs.count(), 2);
    return;
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::OneTablePerHierarchyInsert() {
  try {

  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(true);
}

void QueryResultTestTest::TablePerHierarchySelectWithReference() {
  try {
    QSharedPointer<SubClassS3> subS3 = session.getById<SubClassS3>(5);
    if (subS3->getRef() && subS3->getRef().objectCast<SubClassS1>()) {
      QVERIFY(true);
      return;
    }
  } catch (QtOrm::Exception &e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::enableLogSql() {
  connect(&session, &Session::executedSql, [](QString sql) { qDebug() << sql; });
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
  ConfigurationMap::addMappings<AMap, KindAMap, TypeAMap, BMap, CMap, DMap, EMap, SuperClassSMap, SubClassS1Map,
                                SubClassS2Map, SubClassS3Map>();
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
