#ifndef GROUPCONDITIONS_H
#define GROUPCONDITIONS_H

#include <QObject>

#include "Condition.h"

enum class GroupOperation { And, Or };

class GroupConditions : public QObject {
  Q_OBJECT
public:
  explicit GroupConditions(QObject *parent = nullptr);
  GroupConditions(const GroupConditions &group);

  GroupOperation getOperation() const;
  void setOperation(const GroupOperation &value);

  QList<Condition *> getConditions() const;
  void setConditions(const QList<Condition *> &value);

  QList<GroupConditions *> getGroups() const;
  void setGroups(const QList<GroupConditions *> &value);

  void addGroup(const GroupConditions &value);
  void addCondition(const Condition &value);
  void addCondition(const QString &property, const Operation &operation, const QVariant &value);
  void addConditionEqual(const QString &fieldName, const QVariant &value);
  void removeFilter(Condition *value);
  void clearConditions();
  void clearGroups();
  void clear();
  bool isEmpty() const;

  GroupConditions &operator=(const GroupConditions &group);

private:
  GroupOperation operation = GroupOperation::And;
  QList<Condition *> conditions;
  QList<GroupConditions *> groups;
};

#endif // GROUPCONDITIONS_H
