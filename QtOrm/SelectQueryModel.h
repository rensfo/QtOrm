#ifndef SELECTQUERYMODEL_H
#define SELECTQUERYMODEL_H

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class SelectQueryModel : public QueryModel {
  Q_OBJECT
public:
  explicit SelectQueryModel(QObject *parent = nullptr);

  QString getSelect() const;
  QString getFrom() const;

  GroupConditions getConditions() const;
  void setConditions(const GroupConditions &value);

  QString getWhere() const;

  QueryTableModel *buildQueryTableModel(ClassMapBase *classBase);

  virtual QString getSqlText();

protected:
  virtual void buildModel() override;
  void buildSelectAndFromClause();
  void setAliases(QueryTableModel *tableModel);
  QString generateTableAlias();
  void resetTableNumber();
  QString buildSelectClause();
  QString buildFromClause();
  QString buildWhereClause();
  QString GroupConditionToString(const GroupConditions &conditions);
  QString groupOperationToString(GroupOperation groupOperation) const;
  virtual QString getLikeCondition(const QString &fieldName) const;
  QString operationToString(const Condition &filter) const;

protected:
  int tableNumber = 0;
  const QString tableAliasTemplate = "tb_%1";
  QString select;
  QString from;
  QString where;
  GroupConditions conditions;
};
}
}
#endif // SELECTQUERYMODEL_H
