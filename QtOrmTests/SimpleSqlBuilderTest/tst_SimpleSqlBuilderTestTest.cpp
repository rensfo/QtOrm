#include <QString>
#include <QtTest>

class SimpleSqlBuilderTestTest : public QObject
{
    Q_OBJECT

public:
    SimpleSqlBuilderTestTest();

private Q_SLOTS:
    void testCase1();
};

SimpleSqlBuilderTestTest::SimpleSqlBuilderTestTest()
{
}

void SimpleSqlBuilderTestTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SimpleSqlBuilderTestTest)

#include "tst_SimpleSqlBuilderTestTest.moc"
