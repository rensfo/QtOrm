#ifndef QUERYTABLEMODEL_H
#define QUERYTABLEMODEL_H

#include <QMap>
#include <QObject>

#include "QueryJoin.h"

namespace QtOrm {
namespace Sql {

class QueryTableModel : public QObject {
  Q_OBJECT
public:
  explicit QueryTableModel(QObject *parent = nullptr);

  void appendJoin(QueryJoin *value);
  void removeJoin(QueryJoin *value);
  void clearJoins();

  void addColumn(const QString &name);

  QList<QueryJoin *> getJoins() const;

  QStringList getColumns() const;

  QString getName() const;
  void setName(const QString &value);

  QString getAlias() const;
  void setAlias(const QString &value);

  QString getColumnsForSelectClause();
  QString getTablesForFromClause();
  QString getJoinsText();

  QueryJoin *findJoinByColumnName(const QString &columnName);

protected:
  QString getSelfColumnsForSelectClause();
  QString getJoinColumnsForSelectClause();
  QString joinColumns(const QString &columns1, const QString &columns2);
  QString joinTables(const QString &table1, const QString &table2);
  QString joinStrings(const QString &str1, const QString &str2, const QString &joinString);
  QString ToString(JoinType joinType);


  QString getSelfTableForFromClause();

protected:
  QString name;
  QString alias;
  QStringList columns;

  QList<QueryJoin *> joins;
};
}
}

#endif // QUERYTABLEMODEL_H
