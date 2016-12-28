#ifndef CLASSMAPBASE_H
#define CLASSMAPBASE_H

#include <QHash>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "Exception.h"
#include "OneToMany.h"
#include "OneToOne.h"
#include "PropertyMap.h"

namespace QtOrm {
namespace Mapping {

class ClassMapBase : public QObject {
  Q_OBJECT
public:
  explicit ClassMapBase(QObject *parent = nullptr);

  QString getTable() const;
  void setTable(const QString &table);

  QString getClassName() const;

  const QMap<QString, QSharedPointer<PropertyMap>> getProperties() const;

  PropertyMap &id(QString propertyName);
  PropertyMap &map(QString propertyName);
  PropertyMap &map(QString propertyName, QString columnName);

  QMetaObject getMetaObject() const;
  void setMetaObject(const QMetaObject &classMetaObject);

  QSharedPointer<PropertyMap> getIdProperty() const;
  QSharedPointer<PropertyMap> getProperty(const QString &property);

  OneToMany &oneToMany(const QString &property);
  OneToOne &oneToOne(const QString &property);

  QList<QSharedPointer<OneToMany>> getOneToManyRelations() const;
  QList<QSharedPointer<OneToOne>> getOneToOneRelations() const;

  virtual QVariant getVariantByObjectList(QList<QSharedPointer<QObject>> value) = 0;
  virtual QList<QSharedPointer<QObject>> getObjectListByVariant(QVariant &value) = 0;
  virtual QVariant getVariantByObject(QSharedPointer<QObject> value) = 0;
  virtual QSharedPointer<QObject> getObjectByVariant(QVariant &value) = 0;

  QSharedPointer<OneToOne> findOneToOneByPropertyName(const QString &propertyName);
  QSharedPointer<OneToMany> findOneToManyByPropertyName(const QString &propertyName);

  static QString getTypeNameOfProperty(QSharedPointer<QObject> obj, const QString &prop);
  static QString getTypeNameOfProperty(const QMetaObject &meta, const QString &prop);

  bool propertiesContains(const QString &propertyName);

private:
  PropertyMap &createProperty(QString propertyName);
  void checkToExistProperty(const QString &property);

private:
  QString table;
  QMap<QString, QSharedPointer<PropertyMap>> properties;
  QList<QSharedPointer<OneToMany>> oneToManyRelations;
  QList<QSharedPointer<OneToOne>> oneToOneRelations;

  QString idProperty;

  QMetaObject classMetaObject;
};
}
}

#endif // CLASSMAPBASE_H
