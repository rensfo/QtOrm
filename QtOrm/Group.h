#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>

#include "Condition.h"

enum class GroupOperation { And, Or };

class Group : public QObject {
  Q_OBJECT
public:
  explicit Group(QObject *parent = 0);
  Group(const Group &group);

  GroupOperation getOperation() const;
  void setOperation(const GroupOperation &value);

  QList<Condition *> getFilters() const;
  void setFilters(const QList<Condition *> &value);

  QList<Group *> getGroups() const;
  void setGroups(const QList<Group *> &value);

  void addGroup(const Group &value);
  void addFilter(const Condition &value);
  void addFilter(const QString &fieldName, const Operation &operation, const QVariant &value);
  void addFilterEqual(const QString &fieldName, const QVariant &value);
  void removeFilter(Condition *value);
  void clearFilters();
  void clearGroups();
  void clear();
  bool isEmpty() const;

  Group &operator=(const Group &group);

private:
  GroupOperation operation = GroupOperation::And;
  QList<Condition *> filters;
  QList<Group *> groups;
};

#endif // CONDITION_H
