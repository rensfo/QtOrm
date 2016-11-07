#include "QueryTableModel.h"

namespace QtOrm {
namespace Sql {

QueryTableModel::QueryTableModel(QObject *parent) : QObject(parent) {
}

void QueryTableModel::appendJoin(QueryJoin *value) {
  joins.append(value);
}

void QueryTableModel::removeJoin(QueryJoin *value) {
  joins.removeOne(value);
}

void QueryTableModel::clearJoins() {
  joins.clear();
}

void QueryTableModel::addColumn(const QString &name) {
  columns.append(name);
}

QList<QueryJoin *> QueryTableModel::getJoins() const {
  return joins;
}

QStringList QueryTableModel::getColumns() const {
  return columns;
}

QString QueryTableModel::getName() const {
  return name;
}

void QueryTableModel::setName(const QString &value) {
  name = value;
}

QString QueryTableModel::getAlias() const {
  return alias;
}

void QueryTableModel::setAlias(const QString &value) {
  alias = value;
}

QString QueryTableModel::getColumnsForSelectClause() {
  QString selfColumns = getSelfColumnsForSelectClause(), joinsColumns = getJoinColumnsForSelectClause();

  return joinColumns(selfColumns, joinsColumns);
}

QString QueryTableModel::getTablesForFromClause() {
  QString selfTable = getSelfTableForFromClause(), joinsTables = getJoinsText();

  return selfTable + " " + joinsTables;
}

QString QueryTableModel::getJoinsText() {
  QString joinsText;

  for (QueryJoin *join : joins) {
    QString joinText = QString("%1 %2 %3 on %3.%4 = %5.%6")
                           .arg(ToString(join->getType()))
                           .arg(join->getQueryTableModel()->getName())
                           .arg(join->getQueryTableModel()->getAlias())
                           .arg(join->getRigthTableColumnName())
                           .arg(alias)
                           .arg(join->getLeftTableColumnName());

    QString subJoin = join->getQueryTableModel()->getJoinsText();

    joinText = joinStrings(joinText, subJoin, " ");

    joinsText = joinStrings(joinsText, joinText, " ");
  }

  return joinsText;
}

QueryJoin *QueryTableModel::findJoinByColumnName(const QString &columnName) {
  for (QueryJoin *join : joins) {
    if (join->getLeftTableColumnName() == columnName)
      return join;
  }

  return nullptr;
}

QString QueryTableModel::getSelfColumnsForSelectClause() {
  QString selfColumns;
  for (QString column : columns) {
    QString columnDefinition = QString("%1.%2 as %1_%2").arg(alias, column);
    selfColumns = joinColumns(selfColumns, columnDefinition);
  }

  return selfColumns;
}

QString QueryTableModel::getJoinColumnsForSelectClause() {
  QString joinsColumns;
  for (QueryJoin *join : joins) {
    QString queryJoinColumns = join->getQueryTableModel()->getColumnsForSelectClause();
    joinsColumns = joinColumns(joinsColumns, queryJoinColumns);
  }

  return joinsColumns;
}

QString QueryTableModel::joinColumns(const QString &columns1, const QString &columns2) {
  return joinStrings(columns1, columns2, ", ");
}

QString QueryTableModel::joinTables(const QString &table1, const QString &table2) {
  return joinStrings(table1, table2, "");
}

QString QueryTableModel::joinStrings(const QString &str1, const QString &str2, const QString &joinString) {
  if (str1.isEmpty() && str2.isEmpty())
    return QString();

  if (str1.isEmpty())
    return str2;

  if (str2.isEmpty())
    return str1;

  return str1 + joinString + str2;
}

QString QueryTableModel::ToString(JoinType joinType) {
  if (joinType == JoinType::Left) {
    return "left join";
  } else if (joinType == JoinType::Right) {
    return "right join";
  } else if (joinType == JoinType::FullOuter) {
    return "full outer join";
  }

  return "join";
}

QString QueryTableModel::getSelfTableForFromClause() {
  return name + " " + alias;
}
}
}
