#include <QString>
#include <QtTest>

#include "A.h"
#include "AMap.h"
#include "BMap.h"
#include "ClassMapBase.h"
#include "ConfigurationMap.h"
#include "KindAMap.h"
#include "TypeAMap.h"

#include "Sql.h"

#include "QueryModels/DeleteQueryModel.h"
#include "QueryModels/InsertQueryModel.h"
#include "QueryModels/SelectQueryModel.h"
#include "QueryModels/UpdateQueryModel.h"

#include "Conditions/Condition.h"

using namespace QtOrm;
using namespace Sql;
using namespace Config;

class QueryModelsTestTest : public QObject {
  Q_OBJECT

public:
  QueryModelsTestTest();

private Q_SLOTS:
  void selectClause();
  void fromClause();
  void whereClause();
  void whereClauseUseColumn();
  void emptyWhereClause();
  void oneColumnTwoTimes();
  void updateSql();
  void insertSql();
  void deleteSql();
  void hardWhere();

private:
  void registerClasses();
};

QueryModelsTestTest::QueryModelsTestTest() {
  registerClasses();
}

void QueryModelsTestTest::selectClause() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSelect(), expectedSelectClause);
}

void QueryModelsTestTest::fromClause() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getFrom(), expectedFromClause);
}

void QueryModelsTestTest::whereClause() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;
  group.setOperation(GroupOperation::And);
  group.addEqual(classBase->getIdProperty()->getName(), 1);
  group.addEqual("code", "code1");

  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedWhereClause);
}

void QueryModelsTestTest::whereClauseUseColumn() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;
  group.setOperation(GroupOperation::And);

  group.addEqual(classBase->getIdProperty()->getColumn(), 1);
  group.addEqual("code", "code1");

  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedWhereClause);
}

void QueryModelsTestTest::emptyWhereClause() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;

  query.setConditions(group);

  QCOMPARE(query.getWhere(), QString());
}

void QueryModelsTestTest::oneColumnTwoTimes() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;
  group.setOperation(GroupOperation::Or);
  group.addEqual("code", "code1");
  group.addEqual("code", "code2");

  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedOneColumnTwoTimesWhereClause);
}

void QueryModelsTestTest::updateSql() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  UpdateQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSqlText(), expectedUpdateText);
}

void QueryModelsTestTest::insertSql() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  InsertQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSqlText(), expectedInsertText);
}

void QueryModelsTestTest::deleteSql() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  DeleteQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSqlText(), expectedDeleteText);
}

void QueryModelsTestTest::hardWhere() {
  QSharedPointer<ClassMapBase> classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group =
      (Condition("code_1", QVariant(1)) && Condition("code_1", Operation::Greater, QVariant(2))) ||
      (Condition("code_1", Operation::Equal, QVariant(3)) &&
       (Condition("code_1", Operation::LessOrEqual, QVariant(4)) || Condition("code_1", Operation::NotEqual, QVariant(5))));
  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedHardWhereClause);
}

void QueryModelsTestTest::registerClasses() {
  ConfigurationMap::addMappings<AMap, KindAMap, TypeAMap, BMap>();
}

QTEST_APPLESS_MAIN(QueryModelsTestTest)

#include "tst_QueryModelsTestTest.moc"
