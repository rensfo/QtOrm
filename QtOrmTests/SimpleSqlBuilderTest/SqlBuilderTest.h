#ifndef SQLBUILDERTEST_H
#define SQLBUILDERTEST_H

#include "SimpleSqlBuilder.h"

using namespace QtOrm::Sql;

class SqlBuilderTest : public SimpleSqlBuilder {
public:
  explicit SqlBuilderTest(QObject *parent = nullptr);
  QString buildSelectClause(const QString &className);
  QString buildFromClause(const QString &className);
  QString buildWhereClause(const QString &className);
  QString buildInsert(const QString &className);
  QString buildUpdate(const QString &className);
  QString buildDelete(const QString &className);
};

#endif // SQLBUILDERTEST_H
