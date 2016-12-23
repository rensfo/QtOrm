#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <QVariant>
#include <QSharedPointer>

#include "ClassMapBase.h"
#include "ConfigurationMap.h"

namespace QtOrm {
namespace Mapping {

using QtOrm::Config::ConfigurationMap;

template <typename T>
class ClassMap : public ClassMapBase {
public:
  ClassMap();
  ~ClassMap();
  QVariant getVariantByObjectList(QList<QSharedPointer<QObject>> value) override;
  QVariant getVariantByObject(QSharedPointer<QObject> value) override;
  QSharedPointer<QObject> getObjectByVariant(QVariant &value) override;
  QList<QSharedPointer<QObject>> getObjectListByVariant(QVariant &value) override;
};

template <typename T>
ClassMap<T>::ClassMap() : ClassMapBase() {
  (void)static_cast<QObject *>((T *)0);
  qRegisterMetaType<QSharedPointer<T>>();
  setMetaObject(T::staticMetaObject);
}

template<typename T>
ClassMap<T>::~ClassMap() {

}

template <typename T>
QVariant ClassMap<T>::getVariantByObjectList(QList<QSharedPointer<QObject>> value) {
  QList<QSharedPointer<T>> lst;
  for (QSharedPointer<QObject> &item : value){
    lst.append(item.objectCast<T>());
  }

  return QVariant::fromValue<QList<QSharedPointer<T>>>(lst);
}

template <typename T>
QVariant ClassMap<T>::getVariantByObject(QSharedPointer<QObject> value) {
  return QVariant::fromValue(qobject_cast<T *>(value));
}

template <typename T>
QSharedPointer<QObject> ClassMap<T>::getObjectByVariant(QVariant &value) {
  QSharedPointer<T> sharedData = value.value<QSharedPointer<T>>();
  return sharedData.template objectCast<QObject>();
}

template <typename T>
QList<QSharedPointer<QObject>> ClassMap<T>::getObjectListByVariant(QVariant &value) {
  QList<QSharedPointer<QObject>> ret;

  for (QSharedPointer<T> item : value.value<QList<QSharedPointer<T>>>())
    ret.append(item);
  return ret;
}
}
}

#endif // CLASSMAP_H
