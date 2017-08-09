#include <QSqlError>
#include <QString>
#include <QtTest>

#include "A.h"
#include "AMap.h"
#include "BMap.h"
#include "CMap.h"
#include "Conditions/Condition.h"
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
#include "AOnlyMap.h"
#include "dml.h"
#include "SubClassS1CtiMap.h"
#include "SubClassS2CtiMap.h"
#include "SubClassS3CtiMap.h"

using namespace QtOrm;
using namespace Sql;
using namespace Config;

class QueryResultTestTest : public QObject
{
  Q_OBJECT

public:
  QueryResultTestTest();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  void objectFromRegistry_data();
  void objectFromRegistry();

  void oneTableTwoTimesInQuery_data();
  void oneTableTwoTimesInQuery();

  void oneColumnTwoTimesInWhere_data();
  void oneColumnTwoTimesInWhere();

  void insertObject_data();
  void insertObject();

  void deleteObject_data();
  void deleteObject();

  void updateObject_data();
  void updateObject();

  void where_data();
  void where();

  void orderBy_data();
  void orderBy();

  void refreshObject_data();
  void refreshObject();

  void refreshChildObject_data();
  void refreshChildObject();

  void deleteChildAndRefresh_data();
  void deleteChildAndRefresh();

  void childrenOneToOneParent_data();
  void childrenOneToOneParent();

  void autoUpdate_data();
  void autoUpdate();

  void operationBetween_data();
  void operationBetween();

  void operationIn_data();
  void operationIn();

  void operationGreater_data();
  void operationGreater();

  void operationGreaterOrEqual_data();
  void operationGreaterOrEqual();

  void operationLess_data();
  void operationLess();

  void operationLessOrEqual_data();
  void operationLessOrEqual();

  void operationLike_data();
  void operationLike();

  void nullValueUpdate_data();
  void nullValueUpdate();

  void SingleTableInheritanceSelect_data();
  void SingleTableInheritanceSelect();

  void SingleTableInheritanceConcreteSelect_data();
  void SingleTableInheritanceConcreteSelect();

  void SingleTableInheritanceInsert_data();
  void SingleTableInheritanceInsert();

  void SingleTableInheritanceSelectWithReference_data();
  void SingleTableInheritanceSelectWithReference();

  void ClassTableInheritanceInsert_data();
  void ClassTableInheritanceInsert();

  void ClassTableInheritanceDelete_data();
  void ClassTableInheritanceDelete();

  void ClassTableInheritanceUpdate_data();
  void ClassTableInheritanceUpdate();

  void ClassTableInheritanceAutoUpdate_data();
  void ClassTableInheritanceAutoUpdate();

  void ClassTableInheritanceSelect_data();
  void ClassTableInheritanceSelect();

private:
  void enableLogSql();
  QSqlDatabase openConnection(const QString& name);
  void initDataBase(const QString& dbName, const QStringList& dml);
  template <typename T>
  T find(QList<T> container, std::function<bool(T)> func);

private:
  QtOrm::Session session;
  QSqlQuery query;
  QSqlDatabase db;
};

QueryResultTestTest::QueryResultTestTest()
{
}

void QueryResultTestTest::initTestCase()
{
}

void QueryResultTestTest::cleanupTestCase()
{
}

void QueryResultTestTest::objectFromRegistry_data()
{
  initDataBase("objectFromRegistry", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code1");
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::objectFromRegistry()
{
  try {
    QSharedPointer<A> a = session.getById<A>(1);
    QSharedPointer<A> aFromRegistry = session.getById<A>(1);

    QVERIFY(a == aFromRegistry);
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();

    QVERIFY(false);
  }
}

void QueryResultTestTest::oneTableTwoTimesInQuery_data()
{
  initDataBase("oneTableTwoTimesInQuery", { "create table E(id integer primary key autoincrement, idC integer, idD integer, foreign key (idC)  references  C(id), foreign key (idD)  references D(id))",
                                            "create table C(id integer primary key autoincrement, idKindA integer, foreign key (idKindA)  references KindA(id))",
                                            "create table D(id integer primary key autoincrement, idKindA integer, foreign key (idKindA)  references KindA(id))",
                                            "create table KindA(id integer primary key autoincrement, code text, name text, idTypeA integer, foreign key (idTypeA) references TypeA(id))",
                                            "create table TypeA(id integer primary key autoincrement, code text, name text)"});
  session.clearRegistry();
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<EMap, CMap, DMap, TypeAMap, KindAMap>();

  QSharedPointer<TypeA> typeA = QSharedPointer<TypeA>::create();
  typeA->setCode("code1");
  typeA->setName("value1");
  session.saveObject(typeA);

  QSharedPointer<KindA> kindA = QSharedPointer<KindA>::create();
  kindA->setCode("code1");
  kindA->setName("value1");
  session.saveObject(kindA);

  QSharedPointer<KindA> kindA2 = QSharedPointer<KindA>::create();
  kindA2->setCode("code2");
  kindA2->setName("value2");
  session.saveObject(kindA2);

  QSharedPointer<C> c = QSharedPointer<C>::create();
  c->setKindA(kindA);
  session.saveObject(c);

  QSharedPointer<D> d = QSharedPointer<D>::create();
  d->setKindA(kindA2);
  session.saveObject(d);

  QSharedPointer<E> e = QSharedPointer<E>::create();
  e->setC(c);
  e->setD(d);
  session.saveObject(e);
  session.clearRegistry();
}

void QueryResultTestTest::oneTableTwoTimesInQuery()
{
  try {
    QSharedPointer<E> e = session.getById<E>(1);
    Q_UNUSED(e)

    QVERIFY(true);
  } catch (QtOrm::Exception& e) {
    Q_UNUSED(e)

    QVERIFY(false);
  }
}

void QueryResultTestTest::oneColumnTwoTimesInWhere_data()
{
  initDataBase("oneColumnTwoTimesInWhere", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11", "code22"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::oneColumnTwoTimesInWhere()
{
  try {
    GroupConditions gc;
    gc.setOperation(GroupOperation::Or);
    gc.addEqual("code_1", "code1");
    gc.addEqual("code_1", "code2");

    QList<QSharedPointer<A>> listA = session.getList<A>(gc);

    QCOMPARE(listA.count(), 2);
  } catch (QtOrm::Exception& e) {
    Q_UNUSED(e)

    QVERIFY(false);
  }
}

void QueryResultTestTest::insertObject_data()
{
  initDataBase("insertObject", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
}

void QueryResultTestTest::insertObject()
{
  try
  {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode("code10");
    session.saveObject(a);

    session.clearRegistry();

    a = session.get<A>("code_1", "code10");

    QVERIFY(a);
  }
  catch (QtOrm::Exception& e)
  {
    qDebug().noquote() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::deleteObject_data()
{
  initDataBase("deleteObject", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code1");
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::deleteObject()
{
  try {
    QSharedPointer<A> a = session.getById<A>(1);
    session.deleteObject<A>(a);

    a.clear();
    a = session.getById<A>(1);

    QVERIFY(!a);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::updateObject_data()
{
  initDataBase("updateObject", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code1");
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::updateObject()
{
  try {
    QSharedPointer<A> a = session.getById<A>(1);

    a->setCode("x");
    session.saveObject(a);

    session.clearRegistry();

    a = session.getById<A>(1);

    QCOMPARE(a->getCode(), QString("x"));
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::where_data()
{
  initDataBase("where", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11", "code22"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::where()
{
  GroupConditions where;
  where.addEqual("code_1", "code2");
  auto a = session.getList<A>(where);

  QCOMPARE(a.count(), 1);
}

void QueryResultTestTest::orderBy_data()
{
  initDataBase("orderBy", { "create table TypeA(id integer primary key autoincrement, code text, name text)",
                                  "create table KindA(id integer primary key autoincrement, code text, name text, idTypeA integer, foreign key (idTypeA) references TypeA(id))",
                                  "create table A(id integer primary key autoincrement, code text, idKindA integer, foreign key (idKindA) references KindA(id))",
                                  "create table B(id integer primary key autoincrement, idA integer, code text, foreign key (idA)  references A(id))"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<AMap, BMap, KindAMap, TypeAMap>();

  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code1");

  QList<QSharedPointer<B>> child;
  for(auto c : QStringList{"code2.3", "code2.2", "code2.1"}) {
    QSharedPointer<B> b = QSharedPointer<B>::create();
    b->setCode(c);
    b->setA(a);
    child << b;
  }
  a->setChild(child);
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::orderBy()
{
  try {
    QSharedPointer<A> a = session.getById<A>(1);

    auto child = a->getChild();
    if(child.count()){
      QCOMPARE(child[0]->getCode(), QString("code2.3"));
      QCOMPARE(child[1]->getCode(), QString("code2.2"));
      QCOMPARE(child[2]->getCode(), QString("code2.1"));
    } else {
      QVERIFY(false);
    }
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::refreshObject_data()
{
  initDataBase("refreshObject", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code2");
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::refreshObject()
{
  try {
    QSqlQuery query(db);

    QSharedPointer<A> a = session.get<A>("code_1", "code2");
    if (query.exec("update A set code = null where code = 'code2'")) {
      session.refresh(a);
      QCOMPARE(a->getCode(), QString(""));
      return;
    }
    QVERIFY(false);
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
    QVERIFY(false);
  }
}

void QueryResultTestTest::refreshChildObject_data()
{
  initDataBase("refreshChildObject", { "create table TypeA(id integer primary key autoincrement, code text, name text)",
                                  "create table KindA(id integer primary key autoincrement, code text, name text, idTypeA integer, foreign key (idTypeA) references TypeA(id))",
                                  "create table A(id integer primary key autoincrement, code text, idKindA integer, foreign key (idKindA) references KindA(id))",
                                  "create table B(id integer primary key autoincrement, idA integer, code text, foreign key (idA)  references A(id))"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<AMap, BMap, KindAMap, TypeAMap>();

  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code2");

  QList<QSharedPointer<B>> child;
  for(auto c : QStringList{"code2.3", "code2.2", "code2.1"}) {
    QSharedPointer<B> b = QSharedPointer<B>::create();
    b->setCode(c);
    b->setA(a);
    child << b;
  }
  a->setChild(child);
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::refreshChildObject()
{
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
      return;
    }
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }

  QVERIFY(false);
}

void QueryResultTestTest::deleteChildAndRefresh_data()
{
  initDataBase("deleteChildAndRefresh", { "create table TypeA(id integer primary key autoincrement, code text, name text)",
                                  "create table KindA(id integer primary key autoincrement, code text, name text, idTypeA integer, foreign key (idTypeA) references TypeA(id))",
                                  "create table A(id integer primary key autoincrement, code text, idKindA integer, foreign key (idKindA) references KindA(id))",
                                  "create table B(id integer primary key autoincrement, idA integer, code text, foreign key (idA)  references A(id))"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<AMap, BMap, KindAMap, TypeAMap>();

  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code2");

  QList<QSharedPointer<B>> child;
  for(auto c : QStringList{"code2.3", "code2.2", "code2.1"}) {
    QSharedPointer<B> b = QSharedPointer<B>::create();
    b->setCode(c);
    b->setA(a);
    child << b;
  }
  a->setChild(child);
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::deleteChildAndRefresh()
{
  try
  {
    QSqlQuery query(db);

    QSharedPointer<A> a = session.get<A>("code_1", "code2");
    if (query.exec("delete from B where code = 'code2.2'"))
    {
      session.refresh(a);
      QCOMPARE(a->getChild().count(), 2);
      return;
    }
  }
  catch (QtOrm::Exception& e)
  {
    qDebug() << e.getMessage();
  }

  QVERIFY(false);
}

void QueryResultTestTest::childrenOneToOneParent_data()
{
  initDataBase("childrenOneToOneParent", { "create table TypeA(id integer primary key autoincrement, code text, name text)",
                                  "create table KindA(id integer primary key autoincrement, code text, name text, idTypeA integer, foreign key (idTypeA) references TypeA(id))",
                                  "create table A(id integer primary key autoincrement, code text, idKindA integer, foreign key (idKindA) references KindA(id))",
                                  "create table B(id integer primary key autoincrement, idA integer, code text, foreign key (idA)  references A(id))"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<AMap, BMap, KindAMap, TypeAMap>();

  QSharedPointer<A> a = QSharedPointer<A>::create();
  a->setCode("code2");

  QList<QSharedPointer<B>> child;
  for(auto c : QStringList{"code2.3", "code2.2", "code2.1"}) {
    QSharedPointer<B> b = QSharedPointer<B>::create();
    b->setCode(c);
    b->setA(a);
    child << b;
  }
  a->setChild(child);
  session.saveObject(a);
  session.clearRegistry();
}

void QueryResultTestTest::childrenOneToOneParent()
{
  try {
    QSharedPointer<A> a = session.get<A>("code_1", "code2");

    auto child = a->getChild() ;
    if(child.count()){
      QCOMPARE(a, child.first()->getA().toStrongRef());
    } else {
      QVERIFY(false);
    }
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::autoUpdate_data()
{
  initDataBase("autoUpdate", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(true);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11", "code22"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::autoUpdate()
{
  try {
    QString newCode = "code2.2";
    QString oldCode = "code2";

    auto a = session.get<A>("code_1", oldCode);
    a->setCode(newCode);

    session.clearRegistry();
    a = session.get<A>("code_1", newCode);
    QCOMPARE(a->getCode(), newCode);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationBetween_data()
{
  initDataBase("operationBetween", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11", "code22"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationBetween()
{
  try {
    GroupConditions where;
    where.addBetween("id", 2, 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationIn_data()
{
  initDataBase("operationIn", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11", "code22"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationIn()
{
  try {
    GroupConditions where;
    where.addIn("id", {3, 4});
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationGreater_data()
{
  initDataBase("operationGreater", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationGreater()
{
  try
  {
    GroupConditions where;
    where.addGreater("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 1);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationGreaterOrEqual_data()
{
  initDataBase("operationGreaterOrEqual", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationGreaterOrEqual()
{
  try {
    GroupConditions where;
    where.addGreaterOrEqual("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationLess_data()
{
  initDataBase("operationLess", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationLess()
{
  try {
    GroupConditions where;
    where.addLess("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationLessOrEqual_data()
{
  initDataBase("operationLessOrEqual", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "code3", "code11"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationLessOrEqual()
{
  try {
    GroupConditions where;
    where.addLessOrEqual("id", 3);
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 3);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::operationLike_data()
{
  initDataBase("operationLike", { "create table A(id integer primary key autoincrement, code text, idKindA integer)" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMapping<AOnlyMap>();
  for(auto c : QStringList{"code1", "code2", "co1de", "co2de"}) {
    QSharedPointer<A> a = QSharedPointer<A>::create();
    a->setCode(c);
    session.saveObject(a);
  }
  session.clearRegistry();
}

void QueryResultTestTest::operationLike()
{
  try {
    GroupConditions where;
    where.addLike("code_1", "code");
    auto listA = session.getList<A>(where);

    QCOMPARE(listA.count(), 2);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::nullValueUpdate_data()
{
  initDataBase("nullValueUpdate", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text, int_val integer, str_val text, id_ref integer, foreign key (id_ref)  references super_class_s(id))" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1Map>();
  QSharedPointer<SubClassS1> sub = QSharedPointer<SubClassS1>::create();
  sub->setIntVal(1);
  session.saveObject(sub);
  session.clearRegistry();
}

void QueryResultTestTest::nullValueUpdate()
{
  try {
    auto sub = session.getById<SubClassS1>(1);
    sub->setIntVal(0);
    session.saveObject(sub);
    QSqlQuery query(db);
    if (!query.exec("select int_val from super_class_s where id = 1")) {
      qDebug() << query.lastError().text();
      QVERIFY(false);
      return;
    }

    query.next();
    QCOMPARE(query.value("int_val").isNull(), true);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::SingleTableInheritanceSelect_data()
{
  initDataBase("OneTablePerHierarchySelect", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text, int_val integer, str_val text, id_ref integer, foreign key (id_ref)  references super_class_s(id))" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1Map, SubClassS2Map>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  QSharedPointer<SubClassS1> sub2 = QSharedPointer<SubClassS1>::create();
  sub2->setCode("three");
  sub2->setIntVal(3);
  session.saveObject(sub2);

  QSharedPointer<SubClassS2> sub3 = QSharedPointer<SubClassS2>::create();
  sub3->setCode("two");
  sub3->setStrVal("Два");
  session.saveObject(sub3);

  QSharedPointer<SubClassS2> sub4 = QSharedPointer<SubClassS2>::create();
  sub4->setCode("four");
  sub4->setStrVal("Четыре");
  session.saveObject(sub4);

  session.clearRegistry();
}

void QueryResultTestTest::SingleTableInheritanceSelect()
{
  try
  {
    auto supers = session.getList<SuperClassS>();

    for (auto s : supers) {
      bool result = true;
      QList<long> SubClassS1Ids{1, 2};
      QList<long> SubClassS2Ids{3, 4};
      if (SubClassS1Ids.contains(s->getId())) {
        result = qobject_cast<SubClassS1*>(s);
      } else if (SubClassS2Ids.contains(s->getId())) {
        result = qobject_cast<SubClassS2*>(s);
      } else {
        result = qobject_cast<SubClassS3*>(s);
      }

      if (!result) {
        QVERIFY(false);
        return;
      }
    }
    QVERIFY(true);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::SingleTableInheritanceConcreteSelect_data()
{
  initDataBase("OneTablePerHierarchyConcreteSelect", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text, int_val integer, str_val text, id_ref integer, foreign key (id_ref)  references super_class_s(id))" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1Map>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  QSharedPointer<SubClassS1> sub2 = QSharedPointer<SubClassS1>::create();
  sub2->setCode("three");
  sub2->setIntVal(3);
  session.saveObject(sub2);

  session.clearRegistry();
}

void QueryResultTestTest::SingleTableInheritanceConcreteSelect()
{
  try
  {
    auto subs = session.getList<SubClassS1>();
    QCOMPARE(subs.count(), 2);
    return;
  }
  catch (QtOrm::Exception& e)
  {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::SingleTableInheritanceInsert_data()
{
}

void QueryResultTestTest::SingleTableInheritanceInsert()
{
  try
  {
  }
  catch (QtOrm::Exception& e)
  {
    qDebug() << e.getMessage();
  }
  QVERIFY(true);
}

void QueryResultTestTest::SingleTableInheritanceSelectWithReference_data()
{
  initDataBase("TablePerHierarchySelectWithReference", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text, int_val integer, str_val text, id_ref integer, foreign key (id_ref)  references super_class_s(id))" });
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1Map, SubClassS3Map>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  QSharedPointer<SubClassS3> sub3 = QSharedPointer<SubClassS3>::create();
  sub3->setCode("three");
  sub3->setRef(sub1);
  session.saveObject(sub3);
  session.clearRegistry();
}

void QueryResultTestTest::SingleTableInheritanceSelectWithReference()
{
  try
  {
    QSharedPointer<SubClassS3> subS3 = session.getById<SubClassS3>(2);
    if (subS3->getRef() && subS3->getRef().objectCast<SubClassS1>())
    {
      QVERIFY(true);
      return;
    }
  }
  catch (QtOrm::Exception& e)
  {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::ClassTableInheritanceInsert_data()
{
  initDataBase("ClassTableInheritanceInsert", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text)",
               "create table sub_class_s1(idS1 integer primary key, int_val integer)",
               "create table sub_class_s2(idS2 integer primary key, str_val text)",
               "create table sub_class_s3(idS3 integer primary key, id_ref integer, foreign key (id_ref)  references super_class_s(id))"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1CtiMap, SubClassS2CtiMap, SubClassS3CtiMap>();
}

void QueryResultTestTest::ClassTableInheritanceInsert()
{
  try {
    QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
    sub1->setCode("one");
    sub1->setIntVal(1);
    session.saveObject(sub1);

    QSharedPointer<SubClassS2> sub2 = QSharedPointer<SubClassS2>::create();
    sub2->setCode("two");
    sub2->setStrVal("3");
    session.saveObject(sub2);

    QSharedPointer<SubClassS3> sub3 = QSharedPointer<SubClassS3>::create();
    sub3->setCode("three");
    sub3->setRef(sub1);
    session.saveObject(sub3);

    QVERIFY(true);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::ClassTableInheritanceDelete_data()
{
  initDataBase("ClassTableInheritanceDelete", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text)",
               "create table sub_class_s1(idS1 integer primary key, int_val integer)",
               "create table sub_class_s2(idS2 integer primary key, str_val text)"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1CtiMap, SubClassS2CtiMap>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  QSharedPointer<SubClassS2> sub2 = QSharedPointer<SubClassS2>::create();
  sub2->setCode("three");
  sub2->setStrVal("3");
  session.saveObject(sub2);
  session.clearRegistry();
}

void QueryResultTestTest::ClassTableInheritanceDelete()
{
  try {
    auto supers = session.getList<SuperClassS>();
    if (supers.count() != 2) {
      QVERIFY(false);
      return;
    }
    for(auto item : supers){
      session.deleteObject(item);
    }
    QSqlQuery query(db);
    for(auto table : QStringList{"super_class_s" , "sub_class_s1"}){
      if(!query.exec("select count() cnt from " + table)){
        QVERIFY2(false, query.lastError().text().toStdString().c_str());
        return;
      }

      query.next();
      auto variantValue = query.record().value("cnt");
      int cnt = variantValue.toInt();
      if(cnt != 0) {
        QVERIFY(false);
        return;
      }
    }
    QVERIFY(true);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::ClassTableInheritanceUpdate_data()
{
  initDataBase("ClassTableInheritanceUpdate", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text)",
               "create table sub_class_s1(idS1 integer primary key, int_val integer)",
               "create table sub_class_s2(idS2 integer primary key, str_val text)"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1CtiMap, SubClassS2CtiMap>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  session.clearRegistry();
}

void QueryResultTestTest::ClassTableInheritanceUpdate()
{
  try {
    auto s1 = session.getById<SubClassS1>(1);
    s1->setIntVal(999);
    session.saveObject(s1);
    QSqlQuery query(db);
    if(!query.exec("select int_val from sub_class_s1")){
      QVERIFY2(false, query.lastError().text().toStdString().c_str());
      return;
    }
    query.next();
    auto variantValue = query.record().value("int_val");
    int cnt = variantValue.toInt();
    QVERIFY(cnt == 999);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::ClassTableInheritanceAutoUpdate_data() {
  initDataBase("ClassTableInheritanceAutoUpdate", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text)",
               "create table sub_class_s1(idS1 integer primary key, int_val integer)",
               "create table sub_class_s2(idS2 integer primary key, str_val text)"});
  session.clearRegistry();
  session.setAutoUpdate(true);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1CtiMap, SubClassS2CtiMap>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  session.clearRegistry();
}

void QueryResultTestTest::ClassTableInheritanceAutoUpdate() {
  try {
    auto s1 = session.getById<SubClassS1>(1);
    s1->setCode("new_code");
    s1->setIntVal(999);
    QSqlQuery query(db);
    {
      if(!query.exec("select int_val from sub_class_s1")){
        QVERIFY2(false, query.lastError().text().toStdString().c_str());
        return;
      }
      query.next();
      auto variantValue = query.record().value("int_val");
      int cnt = variantValue.toInt();
      QVERIFY(cnt == 999);
    }
    {
      if(!query.exec("select code from super_class_s")){
        QVERIFY2(false, query.lastError().text().toStdString().c_str());
        return;
      }
      query.next();
      auto variantValue = query.record().value("code");
      QString cnt = variantValue.toString();
      QVERIFY(cnt == "new_code");
    }
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::ClassTableInheritanceSelect_data()
{
  initDataBase("ClassTableInheritanceSelect", { "create table super_class_s(id integer primary key autoincrement, type integer not null, code text)",
               "create table sub_class_s1(idS1 integer primary key, int_val integer)",
               "create table sub_class_s2(idS2 integer primary key, str_val text)",
               "create table sub_class_s3(idS3 integer primary key, id_ref integer, foreign key (id_ref)  references super_class_s(id))"});
  session.clearRegistry();
  session.setAutoUpdate(false);
  ConfigurationMap::removeAllMappings();
  ConfigurationMap::addMappings<SuperClassSMap, SubClassS1CtiMap, SubClassS2CtiMap, SubClassS3CtiMap>();
  QSharedPointer<SubClassS1> sub1 = QSharedPointer<SubClassS1>::create();
  sub1->setCode("one");
  sub1->setIntVal(1);
  session.saveObject(sub1);

  QSharedPointer<SubClassS2> sub2 = QSharedPointer<SubClassS2>::create();
  sub2->setCode("two");
  sub2->setStrVal("3");
  session.saveObject(sub2);

  QSharedPointer<SubClassS3> sub3 = QSharedPointer<SubClassS3>::create();
  sub3->setCode("three");
  sub3->setRef(sub1);
  session.saveObject(sub3);
  session.clearRegistry();
}

void QueryResultTestTest::ClassTableInheritanceSelect()
{
  try {
    auto supers = session.getList<SuperClassS>();
    QVERIFY(supers.count() == 3);
    return;
  } catch (QtOrm::Exception& e) {
    qDebug() << e.getMessage();
  }
  QVERIFY(false);
}

void QueryResultTestTest::enableLogSql()
{
  connect(&session, &Session::executedSql, [](QString sql) { qDebug() << sql; });
}

QSqlDatabase QueryResultTestTest::openConnection(const QString& name)
{
  QSqlDatabase newDb = QSqlDatabase::addDatabase("QSQLITE", name);
  newDb.setDatabaseName(":memory:");
  newDb.open();
  return newDb;
}

void QueryResultTestTest::initDataBase(const QString& dbName, const QStringList& dml)
{
  db = openConnection(dbName);
  if (!db.open()) {
    QFAIL("database do not open");
  }
  session.setDatabase(db);
  QSqlQuery query(db);
  for (auto cmd : dml) {
    bool result = query.exec(cmd);
    if (!result){
      QFAIL(query.lastError().text().toStdString().c_str());
    }
  }
}

template <typename T>
T QueryResultTestTest::find(QList<T> container, std::function<bool(T)> func)
{
  for (T element : container) {
    if (func(element)) {
      return element;
    }
  }
  return T();
}

QTEST_MAIN(QueryResultTestTest)

#include "tst_QueryResultTestTest.moc"
