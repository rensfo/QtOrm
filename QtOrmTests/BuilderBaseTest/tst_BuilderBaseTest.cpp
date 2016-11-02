#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "A.h"
#include "Session.h"
#include "Exception.h"
#include "Sql.h"
#include "SqlBuilderTest.h"
#include "AMap.h"
#include "KindAMap.h"
#include "TypeAMap.h"
#include "BMap.h"

using namespace QtOrm;
using namespace Sql;
using namespace Config;

class BuilderBaseTest : public QObject
{
  Q_OBJECT

public:
  explicit BuilderBaseTest(QObject *parent = nullptr);

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void unregisterClass();

  void selectClause();
  void fromClause();
  void whereClause();

  void insertText();
  void updateText();
  void deleteText();

  void tests();

private:
  bool openConnection();
  void configurateSession();
  void registerClasses();
  bool createDataBase(const QString &dbName);
  bool connectToNewDb(const QString &dbName);
  bool createTables();
  bool fillData();

  bool dropDatabase(const QString &dbName);
  void closeConnection();

private:
  QtOrm::Session session;
  QSqlDatabase db;
  QSqlQuery query;
  QString dbName = "QtOrmTests";
};

BuilderBaseTest::BuilderBaseTest(QObject *parent) : QObject(parent) {}

void BuilderBaseTest::initTestCase()
{
  QVERIFY(openConnection());
  QVERIFY(createDataBase(dbName));
  QVERIFY(connectToNewDb(dbName));
  QVERIFY(createTables());
  QVERIFY(fillData());

  configurateSession();
}

bool BuilderBaseTest::openConnection()
{
  db = QSqlDatabase::addDatabase("QPSQL");
  db.setHostName("192.168.111.250");
  db.setUserName("postgres");
  db.setPassword("postgres");

  bool openResult = db.open();
  query = QSqlQuery(db);

  return openResult;
}

bool BuilderBaseTest::createDataBase(const QString &dbName)
{
  return query.exec(QString("create database %1").arg(dbName));
}

bool BuilderBaseTest::connectToNewDb(const QString &dbName)
{
  db.close();
  db.setDatabaseName(dbName.toLower());

  bool openResult = db.open();
  query = QSqlQuery(db);

  return openResult;
}

bool BuilderBaseTest::createTables() { return query.exec(sqlCreate); }

bool BuilderBaseTest::fillData() { return query.exec(sqlFill); }

void BuilderBaseTest::configurateSession()
{
  session.setDatabase(db);
  session.setSqlBuilderType(SqlBuilderType::Simple);
}

void BuilderBaseTest::registerClasses()
{
  ConfigurationMap::addMapping<AMap>();
  ConfigurationMap::addMapping<KindAMap>();
  ConfigurationMap::addMapping<TypeAMap>();
  ConfigurationMap::addMapping<BMap>();
}

void BuilderBaseTest::cleanupTestCase()
{
  QVERIFY(connectToNewDb(""));
  QVERIFY(dropDatabase(dbName));
  closeConnection();
}

bool BuilderBaseTest::dropDatabase(const QString &dbName)
{
  return query.exec(QString("drop database %1").arg(dbName));
}

void BuilderBaseTest::closeConnection() { db.close(); }

void BuilderBaseTest::unregisterClass()
{
  try
  {
    auto res = session.getList<A>();
    Q_UNUSED(res)
  }
  catch (Exception ex)
  {
    QVERIFY(ex.getGroup() == ErrorGroup::MetaData);
    return;
  }
  QVERIFY(false);
}

void BuilderBaseTest::selectClause()
{
  registerClasses();
  SqlBuilderTest builderTest(this);
  QString selectText = builderTest.buildSelectClause("A");
  QCOMPARE(selectText, expectedSelectClause);
}

void BuilderBaseTest::fromClause()
{
  SqlBuilderTest builderTest(this);
  QString fromText = builderTest.buildFromClause("A");
  QCOMPARE(fromText, expectedFromClause);
}

void BuilderBaseTest::whereClause()
{
  SqlBuilderTest builderTest(this);
  QString whereText = builderTest.buildWhereClause("A");
  QCOMPARE(whereText, expectedWhereClause);
}

void BuilderBaseTest::insertText()
{
  SqlBuilderTest builderTest(this);
  QString insertText = builderTest.buildInsert("A");
  QCOMPARE(insertText, expectedInsertText);
}

void BuilderBaseTest::updateText()
{
  SqlBuilderTest builderTest(this);
  QString updateText = builderTest.buildUpdate("A");
  QCOMPARE(updateText, expectedUpdateText);
}

void BuilderBaseTest::deleteText()
{
  SqlBuilderTest builderTest(this);
  QString deleteText = builderTest.buildDelete("A");
  QCOMPARE(deleteText, expectedDeleteText);
}

void BuilderBaseTest::tests()
{
  try
  {
    A *a = session.getById<A>(1);
    KindA *kind = session.getById<KindA>(2);
    a->setKindA(kind);
  }
  catch (Exception ex)
  {
    qDebug() << ex.getMessage();
  }
  catch (...)
  {
    qDebug() << "other";
  }
}

QTEST_MAIN(BuilderBaseTest)

#include "tst_BuilderBaseTest.moc"
