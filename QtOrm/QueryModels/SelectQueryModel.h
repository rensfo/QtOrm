#ifndef SELECTQUERYMODEL_H
#define SELECTQUERYMODEL_H

#include <QMap>

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class SelectQueryModel : public QueryModel {
public:
  SelectQueryModel();
  ~SelectQueryModel();

  QString getSelect() const;
  QString getFrom() const;

  GroupConditions getConditions() const;
  void setConditions(const GroupConditions &value);

  QString getWhere() const;

  QSharedPointer<QueryTableModel> buildQueryTableModel(QSharedPointer<ClassMapBase> classBase);
  QSharedPointer<QueryTableModel> buildQueryTableModelOneToOne(QSharedPointer<ClassMapBase> classBase);

  virtual QString getSqlText() override;

  QMap<QSharedPointer<Condition>, QString> getConditionPlaceholder() const;
  void clearPlaceHolders();

  virtual void buildModel() override;

  QList<OrderColumn> getOrderColumns() const;
  void setOrderColumns(const QList<OrderColumn> &value);

protected:
  void buildSelectAndFromClause();
  void setAliases(QSharedPointer<QueryTableModel> tableModel);
  QString generateTableAlias();
  void resetTableNumber();
  QString buildSelectClause();
  QString buildFromClause();
  QString buildWhereClause();
  QString buildOrderByClause();
  QString orderColumnToString(OrderColumn &orderColumn);
  QString groupConditionToString(const QSharedPointer<GroupConditions> &conditions);
  QString conditionToString(QSharedPointer<Condition> &condition);
  QString conditionToStringBase(QSharedPointer<Condition> &condition, const QString tableAlias, const QString &placeholder);
  short calculateCountUsedColumn(const QString &value);

protected:
  int tableNumber = 0;
  const QString tableAliasTemplate = "tb_%1";
  const QString selectTemplate = "select %1";
  const QString fromTemplate = "from %1";
  const QString whereTemplate = "where %1";
  const QString orderByTemplate = "order by %1";
  const QString orderColumnTemplate = "%1.%2 %3";
  QString select;
  QString from;
  QString where;
  QString orderBy;
  QList<OrderColumn> orderColumns;
  GroupConditions conditions;
  QMap<QSharedPointer<Condition>, QString> conditionPlaceholder;
};
}
}
#endif // SELECTQUERYMODEL_H
