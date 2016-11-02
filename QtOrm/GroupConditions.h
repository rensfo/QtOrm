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

  QList<Condition *> getFilters() const;
  void setFilters(const QList<Condition *> &value);

  QList<GroupConditions *> getGroups() const;
  void setGroups(const QList<GroupConditions *> &value);

  void addGroup(const GroupConditions &value);
  void addFilter(const Condition &value);
  void addFilter(const QString &fieldName, const Operation &operation, const QVariant &value);
  void addFilterEqual(const QString &fieldName, const QVariant &value);
  void removeFilter(Condition *value);
  void clearFilters();
  void clearGroups();
  void clear();
  bool isEmpty() const;

  GroupConditions &operator=(const GroupConditions &group);

private:
  GroupOperation operation = GroupOperation::And;
  QList<Condition *> filters;
  QList<GroupConditions *> groups;
};

#endif // GROUPCONDITIONS_H
