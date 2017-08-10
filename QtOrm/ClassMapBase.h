#ifndef CLASSMAPBASE_H
#define CLASSMAPBASE_H

#include <QHash>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <qvariant.h>

#include "Exception.h"
#include "PropertyMap.h"
#include "Relations/OneToMany.h"
#include "Relations/OneToOne.h"

namespace QtOrm {
namespace Mapping {

class SubClassMap;

enum class TypeKind {
  Pointer,
  SharedPointer,
  WeakPointer,
  Other
};

enum class InheritanceType {
  None,
  SingleTable,
  ClassTable
};

class ClassMapBase : public QObject {
  Q_OBJECT
public:
  explicit ClassMapBase(QObject *parent = nullptr);

  virtual QString getTable() const;
  void setTable(const QString &table);

  QString getClassName() const;

  virtual QMap<QString, QSharedPointer<PropertyMap>> getProperties();

  PropertyMap &setId(QString propertyName);
  PropertyMap &setId(const QString &propertyName, const QString &columnName);
  PropertyMap &setDiscriminator(const QString &propertyName);
  PropertyMap &setDiscriminator(const QString &propertyName, const QString &columnName);
  PropertyMap &setMap(QString propertyName);
  PropertyMap &setMap(QString propertyName, QString columnName);

  QMetaObject getMetaObject() const;
  void setMetaObject(const QMetaObject &classMetaObject);

  virtual QSharedPointer<PropertyMap> getIdProperty() const;
  virtual QString getIdColumn() const;
  virtual QString getIdPropertyName() const;

  virtual QSharedPointer<PropertyMap> getProperty(const QString &property);
  virtual QString getPropertyColumn(const QString &property);

  OneToMany &setOneToMany(const QString &property);
  OneToOne &setOneToOne(const QString &property);

  virtual QList<QSharedPointer<OneToMany>> getOneToManyRelations() const;
  virtual QList<QSharedPointer<OneToOne>> getOneToOneRelations() const;

  virtual QVariant castToList(TypeKind kind, QList<QSharedPointer<QObject>> value) = 0;
  virtual QVariant castToConcreteSharedPointerList(QList<QSharedPointer<QObject>> value) = 0;
  virtual QVariant castToConcreteWeakPointerList(QList<QSharedPointer<QObject>> value) = 0;
  virtual QVariant castToConcretePointerList(QList<QSharedPointer<QObject>> value) = 0;

  virtual QVariant castTo(TypeKind kind, QSharedPointer<QObject> value) = 0;
  virtual QVariant castToConcreteSharedPointer(QSharedPointer<QObject> value) = 0;
  virtual QVariant castToConcreteWeakPointer(QSharedPointer<QObject> value) = 0;
  virtual QVariant castToConcretePointer(QSharedPointer<QObject> value) = 0;

  virtual QList<QSharedPointer<QObject>> castFromList(TypeKind kind, const QVariant& value) = 0;
  virtual QList<QSharedPointer<QObject>> castFromConcreteSharedPointerList(const QVariant& value) = 0;
  virtual QList<QSharedPointer<QObject>> castFromConcreteWeakPointerList(const QVariant& value) = 0;
  virtual QList<QSharedPointer<QObject>> castFromConcretePointerList(const QVariant& value) = 0;

  virtual QSharedPointer<QObject> castFrom(TypeKind kind, const QVariant& value) = 0;
  virtual QSharedPointer<QObject> castFromConcreteSharedPointer(const QVariant& value) = 0;
  virtual QSharedPointer<QObject> castFromConcreteWeakPointer(const QVariant& value) = 0;
  virtual QSharedPointer<QObject> castFromConcretePointer(const QVariant& value) = 0;

  virtual QSharedPointer<OneToOne> findOneToOneByPropertyName(const QString &propertyName);
  virtual QSharedPointer<OneToMany> findOneToManyByPropertyName(const QString &propertyName);

  static QString getTypeNameOfProperty(QSharedPointer<QObject> obj, const QString &prop);
  static QString getTypeNameOfProperty(const QMetaObject &meta, const QString &prop);

  static TypeKind getTypeKindOfProperty(QSharedPointer<QObject> &obj, const QString &prop);
  static TypeKind getTypeKindOfProperty(const QMetaObject &meta, const QString &prop);

  virtual bool containsProperty(const QString &propertyName) const;
  virtual bool isSubclass();
  SubClassMap* toSubclass();

  virtual QString getDiscriminatorPropertyName() const;
  virtual QString getDiscriminatorColumn() const;
  void setDiscriminatorProperty(const QString &value);
  virtual QSharedPointer<PropertyMap> getDiscriminatorProperty() const;

  QStringList getColumns();

  QVariant getDiscriminatorValue() const;
  void setDiscrimanitorValue(const QVariant &value);

  QMetaObject getClassMetaObject() const;
  void setClassMetaObject(const QMetaObject&value);

  QList<QSharedPointer<ClassMapBase> > getDerrivedClasses() const;
  void setDerrivedClasses(const QList<QSharedPointer<ClassMapBase>>&value);
  void appendDerrivedClass(QSharedPointer<ClassMapBase> value);
  void appendDerrivedClass(QList<QSharedPointer<ClassMapBase>>& values);
  void removeDerrivedClass(QSharedPointer<ClassMapBase> value);

  bool isBaseClass();
  InheritanceType getInheritanceType();

protected:
  static QString getPropertyType(const QMetaObject &meta, const QString &prop);
  virtual void checkProperty(const QString &propertyName) = 0;
  virtual void checkRelationProperty(const QString &propertyName) = 0;

private:
  PropertyMap &createProperty(QString propertyName);

protected:
  QVariant discrimanatorValue;

private:
  QString table;
  QMap<QString, QSharedPointer<PropertyMap>> properties;
  QList<QSharedPointer<OneToMany>> oneToManyRelations;
  QList<QSharedPointer<OneToOne>> oneToOneRelations;
  QList<QSharedPointer<ClassMapBase>> derrivedClasses;

  QString idProperty;
  QString discriminatorProperty;

  QMetaObject classMetaObject;
};
}
}

#endif // CLASSMAPBASE_H
