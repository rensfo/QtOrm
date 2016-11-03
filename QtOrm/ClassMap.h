#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <QVariant>

#include "ClassMapBase.h"

namespace QtOrm {
namespace Mapping {

template <class T>
class ClassMap : public ClassMapBase {
public:
  ClassMap();
  QVariant getVariantByObjectList(QList<QObject *> *value) override;
  QVariant getVariantByObject(QObject *value) override;
  QObject *getObjectByVariant(QVariant &value) override;
  QList<QObject *> *getObjectListByVariant(QVariant &value) override;
};

template <class T>
ClassMap<T>::ClassMap() : ClassMapBase() {
  (void)static_cast<QObject *>((T *)0);
  qRegisterMetaType<T *>();
  setMetaObject(T::staticMetaObject);
}

template <class T>
QVariant ClassMap<T>::getVariantByObjectList(QList<QObject *> *value) {
  QList<T *> *lst = new QList<T *>();
  for (auto item : *value)
    lst->append(static_cast<T *>(item));

  return QVariant::fromValue<QList<T *>>(*lst);
}

template <class T>
QVariant ClassMap<T>::getVariantByObject(QObject *value) {
  return QVariant::fromValue(qobject_cast<T *>(value));
}

template <class T>
QObject *ClassMap<T>::getObjectByVariant(QVariant &value) {
  return value.value<T *>();
}

template <class T>
QList<QObject *> *ClassMap<T>::getObjectListByVariant(QVariant &value) {
  QList<QObject *> *ret = new QList<QObject *>();
  for (auto item : value.value<QList<T *>>())
    ret->append(item);
  return ret;
}
}
}

#endif // CLASSMAP_H
