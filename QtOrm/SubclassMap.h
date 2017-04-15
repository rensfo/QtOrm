#ifndef SUBCLASSMAP_H
#define SUBCLASSMAP_H

#include "ClassMap.h"

namespace QtOrm
{
namespace Mapping
{

template <typename SUPER, typename T>
class SubclassMap : public ClassMap<T>
{
public:
  SubclassMap();
  ~SubclassMap();

  virtual QString getTable() const override;

  virtual QSharedPointer<PropertyMap> getIdProperty() const override;
  virtual QString getColumnIdProperty() const override;
  virtual QString getIdPropertyName() const override;

  virtual QSharedPointer<PropertyMap> getProperty(const QString& property) override;
  virtual QString getPropertyColumn(const QString& property) override;

  virtual QList<QSharedPointer<OneToMany>> getOneToManyRelations() const override;
  virtual QList<QSharedPointer<OneToOne>> getOneToOneRelations() const override;

  virtual QSharedPointer<OneToOne> findOneToOneByPropertyName(const QString& propertyName);
  virtual QSharedPointer<OneToMany> findOneToManyByPropertyName(const QString& propertyName);

  virtual bool containsProperty(const QString& propertyName) const override;
  virtual QMap<QString, QSharedPointer<PropertyMap>> getProperties() override;
};

template <typename SUPER, typename T>
SubclassMap<SUPER, T>::SubclassMap() : ClassMap<T>()
{
  this->setSuperClass(ConfigurationMap::getMappedClass(SUPER::staticMetaObject.className()));
}

template <typename SUPER, typename T>
SubclassMap<SUPER, T>::~SubclassMap()
{
}

template<typename SUPER, typename T>
QString SubclassMap<SUPER, T>::getTable() const
{
  QString tableName = ClassMap<T>::getTable();
  if(tableName.isEmpty())
    return this->getSuperClass()->getTable();

  return tableName;
}

template <typename SUPER, typename T>
QSharedPointer<PropertyMap> SubclassMap<SUPER, T>::getIdProperty() const
{
  return this->getSuperClass()->getIdProperty();
}

template <typename SUPER, typename T>
QString SubclassMap<SUPER, T>::getColumnIdProperty() const
{
  return this->getSuperClass()->getColumnIdProperty();
}

template <typename SUPER, typename T>
QString SubclassMap<SUPER, T>::getIdPropertyName() const
{
  return this->getSuperClass()->getIdPropertyName();
}

template <typename SUPER, typename T>
QSharedPointer<PropertyMap> SubclassMap<SUPER, T>::getProperty(const QString& property)
{
  if (this->containsProperty(property))
    return ClassMap<T>::getProperty(property);

  return this->getSuperClass()->getProperty(property);
}

template <typename SUPER, typename T>
QString SubclassMap<SUPER, T>::getPropertyColumn(const QString& property)
{
  if (ClassMap<T>::containsProperty(property))
    return ClassMap<T>::getPropertyColumn(property);

  return this->getSuperClass()->getPropertyColumn(property);
}

template <typename SUPER, typename T>
QList<QSharedPointer<OneToMany>> SubclassMap<SUPER, T>::getOneToManyRelations() const
{
  auto results = ClassMap<T>::getOneToManyRelations();
  results.append(this->getSuperClass()->getOneToManyRelations());

  return results;
}

template <typename SUPER, typename T>
QList<QSharedPointer<OneToOne>> SubclassMap<SUPER, T>::getOneToOneRelations() const
{
  auto results = ClassMap<T>::getOneToOneRelations();
  results.append(this->getSuperClass()->getOneToOneRelations());

  return results;
}

template <typename SUPER, typename T>
QSharedPointer<OneToOne> SubclassMap<SUPER, T>::findOneToOneByPropertyName(const QString& propertyName)
{
  auto oneToOne = ClassMap<T>::findOneToOneByPropertyName(propertyName);
  if (!oneToOne)
    this->getSuperClass()->findOneToOneByPropertyName(propertyName);

  return oneToOne;
}

template <typename SUPER, typename T>
QSharedPointer<OneToMany> SubclassMap<SUPER, T>::findOneToManyByPropertyName(const QString& propertyName)
{
  auto oneToMany = ClassMap<T>::findOneToManyByPropertyName(propertyName);
  if (!oneToMany)
    this->getSuperClass()->findOneToManyByPropertyName(propertyName);

  return oneToMany;
}

template <typename SUPER, typename T>
bool SubclassMap<SUPER, T>::containsProperty(const QString& propertyName) const
{
  return ClassMap<T>::containsProperty(propertyName) || this->getSuperClass()->containsProperty(propertyName);
}

template<typename SUPER, typename T>
QMap<QString, QSharedPointer<PropertyMap> > SubclassMap<SUPER, T>::getProperties()
{
  auto properties = ClassMap<T>::getProperties();
  auto superProperties = this->getSuperClass()->getProperties();
  for(auto key : superProperties.keys())
    properties[key] = superProperties.value(key);

  return properties;
}
}
}
#endif // SUBCLASSMAP_H
