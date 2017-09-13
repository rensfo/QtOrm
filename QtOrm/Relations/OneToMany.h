#ifndef ONETOMANY_H
#define ONETOMANY_H

#include <GroupConditions.h>
#include <QMetaObject>
#include <QString>

#include "../QueryModels/OrderColumn.h"
#include "Relation.h"

namespace QtOrm {
namespace Mapping {

class OneToMany : public Relation {
  Q_OBJECT
public:
  explicit OneToMany(QObject *parent = nullptr);

  OneToMany &setProperty(const QString &property);

  QString getColumn() const;
  OneToMany &setColumn(const QString &column);

  OneToMany &setSaveCascade(bool value);
  OneToMany &setDeleteCascade(bool value);
  OneToMany &setOrderBy(const QString &property, Sort sort = Sort::ASC);
  OneToMany &setOrderBy(const QList<OrderColumn> &orderBy);
  QList<OrderColumn> getOrderBy();

  OneToMany &setCondition(const Sql::Condition &filter);
  Sql::GroupConditions &getCondition();

private:
  QString refClass;
  QString column;
  QList<OrderColumn> orderBy;
  Sql::GroupConditions group;
};
}
}
#endif // ONETOMANY_H
