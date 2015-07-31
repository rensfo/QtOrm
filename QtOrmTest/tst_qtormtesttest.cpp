#include <QString>
#include <QtTest>

class QtOrmTestTest : public QObject
{
    Q_OBJECT

public:
    QtOrmTestTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1_data();
    void testCase1();
};

QtOrmTestTest::QtOrmTestTest()
{
}

void QtOrmTestTest::initTestCase()
{
}

void QtOrmTestTest::cleanupTestCase()
{
}

void QtOrmTestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void QtOrmTestTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(QtOrmTestTest)

#include "tst_qtormtesttest.moc"
