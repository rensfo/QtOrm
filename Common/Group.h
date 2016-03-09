#ifndef GROUP_H
#define GROUP_H

#include <QObject>

#include "Filter.h"

enum class GroupOperation { And, Or };

class Group : public QObject
{
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

    void addGroup(Group * const &value);
    void addFilter(Filter * const &value);
    void removeFilter(Filter * value);
    bool isEmpty() const;
signals:

public slots:

private:
    GroupOperation operation;
    QList<Filter *> filters;
    QList<Group *> groups;
};

#endif // GROUP_H
