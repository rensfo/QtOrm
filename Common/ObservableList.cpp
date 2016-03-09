#include "ObservableList.h"

ObservableList::ObservableList(QObject *parent)
    : QObject(parent), QObjectList()
{

}

ObservableList::ObservableList(const QObjectList &objList)
{
    QObjectList::clear();
    for(auto object : objList)
        QObjectList::append(object);
}

void ObservableList::append(QObject *object)
{
    QObjectList::append(object);
    emit countChanged();
}

void ObservableList::insert(int i, QObject * const &value)
{
    QObjectList::insert(i, value);
    emit inserted(i, 1);
}

void ObservableList::removeAt(int i)
{
    QObjectList::removeAt(i);
    emit removed(i, 1);
}

void ObservableList::clear()
{
    clear();
    emit cleared();
}

