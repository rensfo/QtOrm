#ifndef SELECTQUERYMODEL_H
#define SELECTQUERYMODEL_H

#include <QMap>

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class SelectQueryModel : public QueryModel {
  Q_OBJECT
public:
  explicit SelectQueryModel(QObject *parent = nullptr);
  ~SelectQueryModel();

  QString getSelect() const;
  QString getFrom() const;

  GroupConditions getConditions() const;
  void setConditions(const GroupConditions &value);

  QString getWhere() const;

  QSharedPointer<QueryTableModel> buildQueryTableModel(QSharedPointer<ClassMapBase> classBase);

  virtual QString getSqlText() override;

  QMap<QSharedPointer<Condition>, QString> getConditionPlaceholder() const;
  void clearPlaceHolders();

  virtual void buildModel() override;

protected:
  void buildSelectAndFromClause();
  void setAliases(QSharedPointer<QueryTableModel> tableModel);
  QString generateTableAlias();
  void resetTableNumber();
  QString buildSelectClause();
  QString buildFromClause();
  QString buildWhereClause();
  QString GroupConditionToString(const QSharedPointer<GroupConditions> &conditions);
  QString groupOperationToString(GroupOperation groupOperation) const;
  virtual QString getLikeCondition(const QString &fieldName) const;
  QString conditionToString(QSharedPointer<Condition> &condition);
  QString conditionToStringBase(QSharedPointer<Condition> &condition, const QString tableAlias, const QString &placeholder);
  short calculateCountUsedColumn(const QString &value);

protected:
  int tableNumber = 0;
  const QString tableAliasTemplate = "tb_%1";
  QString select;
  QString from;
  QString where;
  GroupConditions conditions;
  QMap<QSharedPointer<Condition>, QString> conditionPlaceholder;
};
}
}
#endif // SELECTQUERYMODEL_H
