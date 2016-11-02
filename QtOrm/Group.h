#ifndef GROUP_H
#define GROUP_H

#include <QObject>

#include "Filter.h"

enum class GroupOperation { And, Or };

class Group : public QObject {
  Q_OBJECT
public:
  explicit Group(QObject *parent = 0);
  Group(const Group &group);

  GroupOperation getOperation() const;
  void setOperation(const GroupOperation &value);

  QList<Filter *> getFilters() const;
  void setFilters(const QList<Filter *> &value);

  QList<Group *> getGroups() const;
  void setGroups(const QList<Group *> &value);

  void addGroup(const Group &value);
  void addFilter(const Filter &value);
  void addFilter(const QString &fieldName, const Operation &operation,
                 const QVariant &value);
  void addFilterEqual(const QString &fieldName, const QVariant &value);
  void removeFilter(Filter *value);
  void clearFilters();
  void clearGroups();
  void clear();
  bool isEmpty() const;

  Group &operator=(const Group &group);

private:
  GroupOperation operation = GroupOperation::And;
  QList<Filter *> filters;
  QList<Group *> groups;
};

#endif // GROUP_H
