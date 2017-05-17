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

enum class TypeKind { Pointer, SharedPointer, WeakPointer };

class ClassMapBase : public QObject {
  Q_OBJECT
public:
  explicit ClassMapBase(QObject *parent = nullptr);

  virtual QString getTable() const;
  void setTable(const QString &table);

  QString getClassName() const;

  virtual QMap<QString, QSharedPointer<PropertyMap>> getProperties();

  PropertyMap &id(QString propertyName);
  PropertyMap &id(const QString &propertyName, const QString &columnName);
  PropertyMap &discriminator(const QString &propertyName);
  PropertyMap &discriminator(const QString &propertyName, const QString &columnName);
  PropertyMap &map(QString propertyName);
  PropertyMap &map(QString propertyName, QString columnName);

  QMetaObject getMetaObject() const;
  void setMetaObject(const QMetaObject &classMetaObject);

  virtual QSharedPointer<PropertyMap> getIdProperty() const;
  virtual QString getIdColumn() const;
  virtual QString getIdPropertyName() const;

  virtual QSharedPointer<PropertyMap> getProperty(const QString &property);
  virtual QString getPropertyColumn(const QString &property);

  OneToMany &oneToMany(const QString &property);
  OneToOne &oneToOne(const QString &property);

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

  virtual QList<QSharedPointer<QObject>> castToQObjectSharedPointerList(QVariant &value) = 0;
  virtual QSharedPointer<QObject> castToQObjectSharedPointer(QVariant &value) = 0;

  virtual QSharedPointer<OneToOne> findOneToOneByPropertyName(const QString &propertyName);
  virtual QSharedPointer<OneToMany> findOneToManyByPropertyName(const QString &propertyName);

  static QString getTypeNameOfProperty(QSharedPointer<QObject> obj, const QString &prop);
  static QString getTypeNameOfProperty(const QMetaObject &meta, const QString &prop);

  static TypeKind getTypeKindOfProperty(QSharedPointer<QObject>&obj, const QString &prop);

  virtual bool containsProperty(const QString &propertyName) const;
  bool isSubclass() const;

  virtual QString getDiscriminatorPropertyName() const;
  virtual QString getDiscrimanatorColumn() const;
  void setDiscriminatorProperty(const QString &value);
  virtual QSharedPointer<PropertyMap> getDiscriminatorProperty() const;

  QString getSuperClassName() const;
  QStringList getColumns();

  QVariant getDiscrimanatorValue() const;
  void setDiscrimanatorValue(const QVariant &value);

  QSharedPointer<ClassMapBase> getSuperClass() const;

protected:
  void setSuperClass(const QSharedPointer<ClassMapBase> &value);
  static QString getPropertyType(const QMetaObject &meta, const QString &prop);

private:
  PropertyMap &createProperty(QString propertyName);
  void checkToExistProperty(const QString &property);

protected:
  QVariant discrimanatorValue;

private:
  QString table;
  QMap<QString, QSharedPointer<PropertyMap>> properties;
  QList<QSharedPointer<OneToMany>> oneToManyRelations;
  QList<QSharedPointer<OneToOne>> oneToOneRelations;

  QString idProperty;
  QString discriminatorProperty;

  QMetaObject classMetaObject;
  QSharedPointer<ClassMapBase> superClass;
};
}
}

#endif // CLASSMAPBASE_H
