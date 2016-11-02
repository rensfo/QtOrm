#ifndef CLASSMAPBASE_H
#define CLASSMAPBASE_H

#include <QHash>
#include <QMap>
#include <QMetaObject>
#include <QObject>
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

  const QMap<QString, PropertyMap *> getProperties() const;

  PropertyMap &id(QString propertyName);
  PropertyMap &map(QString propertyName);
  PropertyMap &map(QString propertyName, QString columnName);

  QMetaObject getMetaObject() const;
  void setMetaObject(const QMetaObject &classMetaObject);

  PropertyMap &getIdProperty() const;
  PropertyMap &getProperty(const QString &property);

  OneToMany &oneToMany(const QString &property);
  OneToOne &oneToOne(const QString &property);

  QList<OneToMany *> getOneToManyRelations() const;
  QList<OneToOne *> getOneToOneRelations() const;

  QString getContext() const;
  void setContext(const QString &value);

  QString getInsertFunction() const;
  void setInsertFunction(const QString &value);

  QString getDeleteFunction() const;
  void setDeleteFunction(const QString &value);

  virtual QVariant getVariantByObjectList(QList<QObject *> *value) = 0;
  virtual QList<QObject *> *getObjectListByVariant(QVariant &value) = 0;
  virtual QVariant getVariantByObject(QObject *value) = 0;
  virtual QObject *getObjectByVariant(QVariant &value) = 0;

  OneToOne *findOneToOneByPropertyName(const QString &propertyName);
  OneToMany *findOneToManyByPropertyName(const QString &propertyName);

private:
  PropertyMap &createProperty(QString propertyName);
  void checkToExistProperty(const QString &property);

private:
  QString table;
  QMap<QString, PropertyMap *> properties;
  QList<OneToMany *> oneToManyRelations;
  QList<OneToOne *> oneToOneRelations;

  QString idProperty;

  QMetaObject classMetaObject;
  QString context;
  QString insertFunction;
  QString deleteFunction;
};
}
}

#endif // CLASSMAPBASE_H
