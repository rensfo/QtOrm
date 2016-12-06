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

#include "SelectQueryModel.h"
#include "DeleteQueryModel.h"
#include "InsertQueryModel.h"
#include "UpdateQueryModel.h"

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

private:
  void registerClasses();
};

QueryModelsTestTest::QueryModelsTestTest() {
  registerClasses();
}

void QueryModelsTestTest::selectClause() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSelect(), expectedSelectClause);
}

void QueryModelsTestTest::fromClause() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getFrom(), expectedFromClause);
}

void QueryModelsTestTest::whereClause() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;
  group.setOperation(GroupOperation::And);
  group.addConditionEqual(classBase->getIdProperty().getName(), 1);
  group.addConditionEqual("code_1", "code1");

  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedWhereClause);
}

void QueryModelsTestTest::whereClauseUseColumn() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;
  group.setOperation(GroupOperation::And);

  group.addConditionEqual(classBase->getIdProperty().getColumn(), 1);

  Condition condition;
  condition.setColumn("code");
  condition.setValue("code1");
  condition.setOperation(Operation::Equal);
  group.addCondition(condition);

  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedWhereClause);
}

void QueryModelsTestTest::emptyWhereClause() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;

  query.setConditions(group);

  QCOMPARE(query.getWhere(), QString());
}

void QueryModelsTestTest::oneColumnTwoTimes() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  SelectQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  GroupConditions group;
  group.setOperation(GroupOperation::Or);
  group.addConditionEqual("code_1", "code1");
  group.addConditionEqual("code_1", "code2");

  query.setConditions(group);

  QCOMPARE(query.getWhere(), expectedOneColumnTwoTimesWhereClause);
}

void QueryModelsTestTest::updateSql() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  UpdateQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSqlText(), expectedUpdateText);
}

void QueryModelsTestTest::insertSql() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  InsertQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSqlText(), expectedInsertText);
}

void QueryModelsTestTest::deleteSql() {
  ClassMapBase *classBase = ConfigurationMap::getMappedClass("A");
  DeleteQueryModel query;
  query.setClassBase(classBase);
  query.buildModel();

  QCOMPARE(query.getSqlText(), expectedDeleteText);
}

void QueryModelsTestTest::registerClasses() {
  ConfigurationMap::addMapping<AMap>();
  ConfigurationMap::addMapping<KindAMap>();
  ConfigurationMap::addMapping<TypeAMap>();
  ConfigurationMap::addMapping<BMap>();
}

QTEST_APPLESS_MAIN(QueryModelsTestTest)

#include "tst_QueryModelsTestTest.moc"
