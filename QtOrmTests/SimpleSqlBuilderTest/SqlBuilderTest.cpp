#include "SqlBuilderTest.h"

#include "ClassMapBase.h"
#include "ConfigurationMap.h"

//using namespace QtOrm::Mapping;
using namespace QtOrm::Config;

SqlBuilderTest::SqlBuilderTest(QObject *parent)
    : SimpleSqlBuilder(parent)
{
}

QString SqlBuilderTest::buildSelectClause(const QString &className)
{
  classBase = ConfigurationMap::getMappedClass(className);
  tableAlias = generateTableAlias();
  fillOneToOneAlias();
  return getSelect(*classBase);
}

QString SqlBuilderTest::buildFromClause(const QString &className)
{
  classBase = ConfigurationMap::getMappedClass(className);
  tableAlias = generateTableAlias();
  fillOneToOneAlias();
  return getFrom(*classBase);
}

QString SqlBuilderTest::buildWhereClause(const QString &className)
{
  classBase = ConfigurationMap::getMappedClass(className);
  Group group;
  group.setOperation(GroupOperation::And);
  group.addFilterEqual(classBase->getIdProperty().getColumn(), 1);
  group.addFilterEqual("code", "code1");
  return getWhere(classBase->getTable(), group);
}

QString SqlBuilderTest::buildInsert(const QString &className)
{
  classBase = ConfigurationMap::getMappedClass(className);
  return getInsertText();
}

QString SqlBuilderTest::buildUpdate(const QString &className)
{
  classBase = ConfigurationMap::getMappedClass(className);
  return getUpdateText();
}

QString SqlBuilderTest::buildDelete(const QString &className)
{
  classBase = ConfigurationMap::getMappedClass(className);
  return getDeleteText();
}
