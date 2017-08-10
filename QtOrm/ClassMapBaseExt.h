#ifndef CLASSMAPBASEEXT_H
#define CLASSMAPBASEEXT_H

#include <QSharedPointer>
#include <QVariant>

#include "ConfigurationMap.h"

namespace QtOrm {
namespace Mapping {

template <typename T, typename BASE>
class ClassMapBaseExt : public BASE {
public:
  ClassMapBaseExt();
  ~ClassMapBaseExt();

  QVariant castToList(TypeKind kind, QList<QSharedPointer<QObject>> value) override;
  QVariant castToConcreteSharedPointerList(QList<QSharedPointer<QObject>> value) override;
  QVariant castToConcreteWeakPointerList(QList<QSharedPointer<QObject>> value) override;
  QVariant castToConcretePointerList(QList<QSharedPointer<QObject>> value) override;

  QVariant castTo(TypeKind kind, QSharedPointer<QObject> value) override;
  QVariant castToConcreteSharedPointer(QSharedPointer<QObject> value) override;
  QVariant castToConcreteWeakPointer(QSharedPointer<QObject> value) override;
  QVariant castToConcretePointer(QSharedPointer<QObject> value) override;

  virtual QList<QSharedPointer<QObject> > castFromList(TypeKind kind, const QVariant&value) override;
  virtual QList<QSharedPointer<QObject> > castFromConcreteSharedPointerList(const QVariant&value) override;
  virtual QList<QSharedPointer<QObject> > castFromConcreteWeakPointerList(const QVariant&value) override;
  virtual QList<QSharedPointer<QObject> > castFromConcretePointerList(const QVariant&value) override;

  virtual QSharedPointer<QObject> castFrom(TypeKind kind, const QVariant&value) override;
  virtual QSharedPointer<QObject> castFromConcreteSharedPointer(const QVariant&value) override;
  virtual QSharedPointer<QObject> castFromConcreteWeakPointer(const QVariant&value) override;
  virtual QSharedPointer<QObject> castFromConcretePointer(const QVariant&value) override;

protected:
  virtual void checkProperty(const QString &propertyName) override;
  virtual void checkRelationProperty(const QString &propertyName) override;
};

template <typename T, typename BASE>
ClassMapBaseExt<T, BASE>::ClassMapBaseExt() : BASE() {
  (void)static_cast<QObject *>((T *)0);
  qRegisterMetaType<QSharedPointer<T>>();
  qRegisterMetaType<QWeakPointer<T>>();
  this->setClassMetaObject(T::staticMetaObject);
}

template <typename T, typename BASE>
ClassMapBaseExt<T, BASE>::~ClassMapBaseExt() {
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToList(TypeKind kind, QList<QSharedPointer<QObject>> value) {
  if (kind == TypeKind::Pointer) {
    return castToConcretePointerList(value);
  } else if (kind == TypeKind::SharedPointer) {
    return castToConcreteSharedPointerList(value);
  }

  return castToConcreteWeakPointerList(value);
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToConcreteSharedPointerList(QList<QSharedPointer<QObject>> value) {
  QList<QSharedPointer<T>> lst;
  for (QSharedPointer<QObject> &item : value) {
    lst.append(item.objectCast<T>());
  }

  return QVariant::fromValue<QList<QSharedPointer<T>>>(lst);
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToConcreteWeakPointerList(QList<QSharedPointer<QObject>> value) {
  QList<QWeakPointer<T>> lst;
  for (QSharedPointer<QObject> &item : value) {
    lst.append(item.objectCast<T>().toWeakRef());
  }

  return QVariant::fromValue(lst);
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToConcretePointerList(QList<QSharedPointer<QObject>> value) {
  QList<T *> lst;
  for (QSharedPointer<QObject> &item : value) {
    lst.append(item.objectCast<T>().data());
  }

  return QVariant::fromValue(lst);
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castTo(TypeKind kind, QSharedPointer<QObject> value) {
  if (kind == TypeKind::Pointer) {
    return castToConcretePointer(value);
  } else if (kind == TypeKind::SharedPointer) {
    return castToConcreteSharedPointer(value);
  }

  return castToConcreteWeakPointer(value);
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToConcreteSharedPointer(QSharedPointer<QObject> value) {
  return QVariant::fromValue(value.objectCast<T>());
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToConcreteWeakPointer(QSharedPointer<QObject> value) {
  return QVariant::fromValue(value.objectCast<T>().toWeakRef());
}

template <typename T, typename BASE>
QVariant ClassMapBaseExt<T, BASE>::castToConcretePointer(QSharedPointer<QObject> value) {
  return QVariant::fromValue(value.objectCast<T>().data());
}

template <typename T, typename BASE>
QList<QSharedPointer<QObject> > ClassMapBaseExt<T, BASE>::castFromList(TypeKind kind, const QVariant&value)
{
  if (kind == TypeKind::Pointer) {
    return castFromConcretePointerList(value);
  } else if (kind == TypeKind::SharedPointer) {
    return castFromConcreteSharedPointerList(value);
  }

  return castFromConcreteWeakPointerList(value);
}

template <typename T, typename BASE>
QList<QSharedPointer<QObject> > ClassMapBaseExt<T, BASE>::castFromConcreteSharedPointerList(const QVariant&value)
{
  QList<QSharedPointer<T>> list = value.value<QList<QSharedPointer<T>>>();
  QList<QSharedPointer<QObject>> result;
  for(QSharedPointer<T> item : list) {
    result << item.template objectCast<QObject>();
  }

  return result;
}

template <typename T, typename BASE>
QList<QSharedPointer<QObject> > ClassMapBaseExt<T, BASE>::castFromConcreteWeakPointerList(const QVariant&value)
{
  QList<QWeakPointer<T>> list = value.value<QList<QWeakPointer<T>>>();
  QList<QSharedPointer<QObject>> result;
  for(auto item : list) {
    result << item.toStrongRef().template objectCast<QObject>();
  }

  return result;
}

template <typename T, typename BASE>
QList<QSharedPointer<QObject>> ClassMapBaseExt<T, BASE>::castFromConcretePointerList(const QVariant&value)
{
  QList<T*> list = value.value<QList<T*>>();
  QList<QSharedPointer<QObject>> result;
  for(auto item : list) {
    result << QSharedPointer<QObject>(item);
  }

  return result;
}

template <typename T, typename BASE>
QSharedPointer<QObject> ClassMapBaseExt<T, BASE>::castFrom(TypeKind kind, const QVariant&value)
{
  if (kind == TypeKind::Pointer) {
    return castFromConcretePointer(value);
  } else if (kind == TypeKind::SharedPointer) {
    return castFromConcreteSharedPointer(value);
  }

  return castFromConcreteWeakPointer(value);
}

template <typename T, typename BASE>
QSharedPointer<QObject> ClassMapBaseExt<T, BASE>::castFromConcreteSharedPointer(const QVariant&value)
{
  QSharedPointer<T> sharedPointer = value.value<QSharedPointer<T>>();
  return sharedPointer.template objectCast<QObject>();
}

template <typename T, typename BASE>
QSharedPointer<QObject> ClassMapBaseExt<T, BASE>::castFromConcreteWeakPointer(const QVariant&value)
{
  QWeakPointer<T> weakPointer = value.value<QWeakPointer<T>>();
  return weakPointer.toStrongRef().template objectCast<QObject>();
}

template <typename T, typename BASE>
QSharedPointer<QObject> ClassMapBaseExt<T, BASE>::castFromConcretePointer(const QVariant&value)
{
  T* pointer = value.value<T*>();
  return QSharedPointer<QObject>(pointer);
}

template <typename T, typename BASE>
void ClassMapBaseExt<T, BASE>::checkProperty(const QString &propertyName) {
  int propertyIndex = T::staticMetaObject.indexOfProperty(propertyName.toStdString().data());
  Q_ASSERT_X(propertyIndex != -1, "checkProperty", QString("Property %2 in class %1 did not found")
                                                       .arg(T::staticMetaObject.className())
                                                       .arg(propertyName)
                                                       .toStdString()
                                                       .data());
}

template <typename T, typename BASE>
void ClassMapBaseExt<T, BASE>::checkRelationProperty(const QString &propertyName) {
  checkProperty(propertyName);

  TypeKind kind = this->getTypeKindOfProperty(T::staticMetaObject, propertyName);
  Q_ASSERT_X(kind != TypeKind::Other, "checkRelationProperty", QString("Class %1 property %2 has not registred type")
                                                                   .arg(T::staticMetaObject.className())
                                                                   .arg(propertyName)
                                                                   .toStdString()
                                                                   .data());
}
}
}

#endif // CLASSMAPBASEEXT_H
