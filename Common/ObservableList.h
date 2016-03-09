#ifndef OBSERVABLELIST_H
#define OBSERVABLELIST_H

#include <QObject>
#include <QObjectList>

class ObservableList : public QObject, public QObjectList
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    ObservableList(QObject *parent = 0);
    ObservableList(const QObjectList &objList);
    void append(QObject *object);
    void insert(int i, QObject * const &value);
    void removeAt(int i);
    void clear();

signals:
    void countChanged();
    void inserted(int index, int count);
    void removed(int index, int count);
    void cleared();

private:
    QObjectList data;
};

#endif // OBSERVABLELIST_H
