#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <QSharedPointer>
#include <QVariant>

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

  QVariant castToList(TypeKind kind, QList<QSharedPointer<QObject>> value) override;
  QVariant castToConcreteSharedPointerList(QList<QSharedPointer<QObject>> value) override;
  QVariant castToConcreteWeakPointerList(QList<QSharedPointer<QObject>> value) override;
  QVariant castToConcretePointerList(QList<QSharedPointer<QObject>> value) override;

  QVariant castTo(TypeKind kind, QSharedPointer<QObject> value) override;
  QVariant castToConcreteSharedPointer(QSharedPointer<QObject> value) override;
  QVariant castToConcreteWeakPointer(QSharedPointer<QObject> value) override;
  QVariant castToConcretePointer(QSharedPointer<QObject> value) override;

  QList<QSharedPointer<QObject>> castToQObjectSharedPointerList(QVariant &value) override;
  QSharedPointer<QObject> castToQObjectSharedPointer(QVariant &value) override;
};

template <typename T>
ClassMap<T>::ClassMap() : ClassMapBase() {
  (void)static_cast<QObject *>((T *)0);
  qRegisterMetaType<QSharedPointer<T>>();
  setMetaObject(T::staticMetaObject);
}

template <typename T>
ClassMap<T>::~ClassMap() {
}

template <typename T>
QVariant ClassMap<T>::castToList(TypeKind kind, QList<QSharedPointer<QObject>> value) {
  if(kind == TypeKind::Pointer){
    return castToConcretePointerList(value);
  } else if(kind == TypeKind::SharedPointer){
    return castToConcreteSharedPointerList(value);
  }

  return castToConcreteWeakPointerList(value);
}

template <typename T>
QVariant ClassMap<T>::castToConcreteSharedPointerList(QList<QSharedPointer<QObject>> value) {
  QList<QSharedPointer<T>> lst;
  for (QSharedPointer<QObject> &item : value) {
    lst.append(item.objectCast<T>());
  }

  return QVariant::fromValue<QList<QSharedPointer<T>>>(lst);
}

template <typename T>
QVariant ClassMap<T>::castToConcreteWeakPointerList(QList<QSharedPointer<QObject>> value) {
  QList<QWeakPointer<T>> lst;
  for (QSharedPointer<QObject> &item : value) {
    lst.append(item.objectCast<T>().toWeakRef());
  }

  return QVariant::fromValue(lst);
}

template <typename T>
QVariant ClassMap<T>::castToConcretePointerList(QList<QSharedPointer<QObject>> value) {
  QList<T *> lst;
  for (QSharedPointer<QObject> &item : value) {
    lst.append(item.objectCast<T>().data());
  }

  return QVariant::fromValue(lst);
}

template <typename T>
QVariant ClassMap<T>::castTo(TypeKind kind, QSharedPointer<QObject> value) {
  if(kind == TypeKind::Pointer){
    return castToConcretePointer(value);
  } else if(kind == TypeKind::SharedPointer){
    return castToConcreteSharedPointer(value);
  }

  return castToConcreteWeakPointer(value);
}

template <typename T>
QVariant ClassMap<T>::castToConcreteSharedPointer(QSharedPointer<QObject> value) {
  return QVariant::fromValue(value.objectCast<T>());
}

template <typename T>
QVariant ClassMap<T>::castToConcreteWeakPointer(QSharedPointer<QObject> value) {
  return QVariant::fromValue(value.objectCast<T>().toWeakRef());
}

template <typename T>
QVariant ClassMap<T>::castToConcretePointer(QSharedPointer<QObject> value) {
  return QVariant::fromValue(value.objectCast<T>().data());
}

template <typename T>
QSharedPointer<QObject> ClassMap<T>::castToQObjectSharedPointer(QVariant &value) {
  QSharedPointer<T> sharedData = value.value<QSharedPointer<T>>();
  return sharedData.template objectCast<QObject>();
}

template <typename T>
QList<QSharedPointer<QObject>> ClassMap<T>::castToQObjectSharedPointerList(QVariant &value) {
  QList<QSharedPointer<QObject>> ret;

  for (QSharedPointer<T> item : value.value<QList<QSharedPointer<T>>>())
    ret.append(item);
  return ret;
}
}
}

#endif // CLASSMAP_H
