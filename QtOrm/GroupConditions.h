#ifndef GROUPCONDITIONS_H
#define GROUPCONDITIONS_H

#include <QObject>
#include <QSharedPointer>

#include "Condition.h"

namespace QtOrm {
namespace Sql {

enum class GroupOperation { And, Or };

class GroupConditions : public QObject {
  Q_OBJECT
public:
  explicit GroupConditions(QObject *parent = nullptr);
  GroupConditions(const GroupConditions &group);

  GroupOperation getOperation() const;
  void setOperation(const GroupOperation &value);

  QList<QSharedPointer<Condition>> getConditions() const;
  void setConditions(const QList<QSharedPointer<Condition>> &value);

  QList<QSharedPointer<GroupConditions>> getGroups() const;
  void setGroups(const QList<QSharedPointer<GroupConditions>> &value);

  void addGroup(const GroupConditions &value);
  void addCondition(const Condition &value);

  void addEqual(const QString &fieldName, const QVariant &value);
  void addNotEqual(const QString &fieldName, const QVariant &value);
  void addLike(const QString &fieldName, const QVariant &value);
  void addBetween(const QString &fieldName, const QVariant &value1, const QVariant &value2);
  void addIn(const QString &fieldName, const QVariantList &values);

  void clearConditions();
  void clearGroups();
  void clear();
  bool isEmpty() const;

  GroupConditions &operator=(const GroupConditions &group);

protected:
  void addCondition(const QString &property, const Operation &operation, const QVariant &value);
  void addCondition(const QString &property, const Operation &operation, const QVariantList &values);

protected:
  GroupOperation operation = GroupOperation::And;
  QList<QSharedPointer<Condition>> conditions;
  QList<QSharedPointer<GroupConditions>> groups;
};
}
}
#endif // GROUPCONDITIONS_H
