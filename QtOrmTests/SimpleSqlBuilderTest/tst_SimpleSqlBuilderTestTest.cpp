#include <QString>
#include <QtTest>

#include "A.h"
#include "Sql.h"
#include "SqlBuilderTest.h"
#include "AMap.h"
#include "KindAMap.h"
#include "TypeAMap.h"
#include "BMap.h"

using namespace QtOrm;
using namespace Sql;
using namespace Config;

class SimpleSqlBuilderTestTest : public QObject
{
    Q_OBJECT

public:
    SimpleSqlBuilderTestTest();

private Q_SLOTS:
    void selectClause();
    void fromClause();
    void whereClause();

    void insertText();
    void updateText();
    void deleteText();

private:
    void registerClasses();
};

SimpleSqlBuilderTestTest::SimpleSqlBuilderTestTest()
{
}


void SimpleSqlBuilderTestTest::selectClause()
{
  registerClasses();
  SqlBuilderTest builderTest(this);
  QString selectText = builderTest.buildSelectClause("A");
  QCOMPARE(selectText, expectedSelectClause);
}

void SimpleSqlBuilderTestTest::fromClause()
{
  SqlBuilderTest builderTest(this);
  QString fromText = builderTest.buildFromClause("A");
  QCOMPARE(fromText, expectedFromClause);
}

void SimpleSqlBuilderTestTest::whereClause()
{
  SqlBuilderTest builderTest(this);
  QString whereText = builderTest.buildWhereClause("A");
  QCOMPARE(whereText, expectedWhereClause);
}

void SimpleSqlBuilderTestTest::insertText()
{
  SqlBuilderTest builderTest(this);
  QString insertText = builderTest.buildInsert("A");
  QCOMPARE(insertText, expectedInsertText);
}

void SimpleSqlBuilderTestTest::updateText()
{
  SqlBuilderTest builderTest(this);
  QString updateText = builderTest.buildUpdate("A");
  QCOMPARE(updateText, expectedUpdateText);
}

void SimpleSqlBuilderTestTest::deleteText()
{
  SqlBuilderTest builderTest(this);
  QString deleteText = builderTest.buildDelete("A");
  QCOMPARE(deleteText, expectedDeleteText);
}

void SimpleSqlBuilderTestTest::registerClasses()
{
    ConfigurationMap::addMapping<AMap>();
    ConfigurationMap::addMapping<KindAMap>();
    ConfigurationMap::addMapping<TypeAMap>();
    ConfigurationMap::addMapping<BMap>();
}

QTEST_APPLESS_MAIN(SimpleSqlBuilderTestTest)

#include "tst_SimpleSqlBuilderTestTest.moc"
