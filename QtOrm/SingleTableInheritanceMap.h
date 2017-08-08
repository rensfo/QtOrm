#ifndef SINGLETABLEINHERITANCEMAP_H
#define SINGLETABLEINHERITANCEMAP_H

#include <QSharedPointer>
#include <QString>

#include "PropertyMap.h"
#include "ClassMapBaseExt.h"
#include "SubClassMap.h"

namespace QtOrm {
namespace Mapping {

template <typename SUPER, typename T>
class SingleTableInheritanceMap : public ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>
{
public:
  SingleTableInheritanceMap();
  ~SingleTableInheritanceMap();

  virtual QString getTable() const override;

  virtual QSharedPointer<PropertyMap> getIdProperty() const override;
  virtual QString getIdColumn() const override;
  virtual QString getIdPropertyName() const override;

  virtual QSharedPointer<PropertyMap> getProperty(const QString &property) override;
  virtual QString getPropertyColumn(const QString &property) override;

  virtual QString getDiscriminatorPropertyName() const;
  virtual QString getDiscriminatorColumn() const;

  virtual QList<QSharedPointer<OneToMany>> getOneToManyRelations() const override;
  virtual QList<QSharedPointer<OneToOne>> getOneToOneRelations() const override;

  virtual QSharedPointer<OneToOne> findOneToOneByPropertyName(const QString &propertyName);
  virtual QSharedPointer<OneToMany> findOneToManyByPropertyName(const QString &propertyName);

  virtual bool containsProperty(const QString &propertyName) const override;
  virtual QMap<QString, QSharedPointer<PropertyMap>> getProperties() override;
};

template <typename SUPER, typename T>
SingleTableInheritanceMap<SUPER, T>::SingleTableInheritanceMap() : ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>() {
  this->setInheritanceType(InheritanceType::SingleTable);
  this->setSuperClass(ConfigurationMap::getMappedClass(SUPER::staticMetaObject.className()));
}

template <typename SUPER, typename T>
SingleTableInheritanceMap<SUPER, T>::~SingleTableInheritanceMap() {
}

template <typename SUPER, typename T>
QString SingleTableInheritanceMap<SUPER, T>::getTable() const {
  QString tableName = ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::getTable();
  if (tableName.isEmpty())
    return this->getSuperClass()->getTable();

  return tableName;
}

template <typename SUPER, typename T>
QSharedPointer<PropertyMap> SingleTableInheritanceMap<SUPER, T>::getIdProperty() const {
  return this->getSuperClass()->getIdProperty();
}

template <typename SUPER, typename T>
QString SingleTableInheritanceMap<SUPER, T>::getIdColumn() const {
  return this->getSuperClass()->getIdColumn();
}

template <typename SUPER, typename T>
QString SingleTableInheritanceMap<SUPER, T>::getIdPropertyName() const {
  return this->getSuperClass()->getIdPropertyName();
}

template <typename SUPER, typename T>
QSharedPointer<PropertyMap> SingleTableInheritanceMap<SUPER, T>::getProperty(const QString &property) {
  if (this->containsProperty(property))
    return ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::getProperty(property);

  return this->getSuperClass()->getProperty(property);
}

template <typename SUPER, typename T>
QString SingleTableInheritanceMap<SUPER, T>::getPropertyColumn(const QString &property) {
  if (ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::containsProperty(property))
    return ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::getPropertyColumn(property);

  return this->getSuperClass()->getPropertyColumn(property);
}

template <typename SUPER, typename T>
QString SingleTableInheritanceMap<SUPER, T>::getDiscriminatorPropertyName() const {
  return this->getSuperClass()->getDiscriminatorPropertyName();
}

template <typename SUPER, typename T>
QString SingleTableInheritanceMap<SUPER, T>::getDiscriminatorColumn() const {
  return this->getSuperClass()->getDiscriminatorColumn();
}

template <typename SUPER, typename T>
QList<QSharedPointer<OneToMany>> SingleTableInheritanceMap<SUPER, T>::getOneToManyRelations() const {
  auto results = ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::getOneToManyRelations();
  results.append(this->getSuperClass()->getOneToManyRelations());

  return results;
}

template <typename SUPER, typename T>
QList<QSharedPointer<OneToOne>> SingleTableInheritanceMap<SUPER, T>::getOneToOneRelations() const {
  auto results = ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::getOneToOneRelations();
  results.append(this->getSuperClass()->getOneToOneRelations());

  return results;
}

template <typename SUPER, typename T>
QSharedPointer<OneToOne> SingleTableInheritanceMap<SUPER, T>::findOneToOneByPropertyName(const QString &propertyName) {
  auto oneToOne = ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::findOneToOneByPropertyName(propertyName);
  if (!oneToOne)
    this->getSuperClass()->findOneToOneByPropertyName(propertyName);

  return oneToOne;
}

template <typename SUPER, typename T>
QSharedPointer<OneToMany> SingleTableInheritanceMap<SUPER, T>::findOneToManyByPropertyName(const QString &propertyName) {
  auto oneToMany = ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::findOneToManyByPropertyName(propertyName);
  if (!oneToMany)
    this->getSuperClass()->findOneToManyByPropertyName(propertyName);

  return oneToMany;
}

template <typename SUPER, typename T>
bool SingleTableInheritanceMap<SUPER, T>::containsProperty(const QString &propertyName) const {
  return ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::containsProperty(propertyName) || this->getSuperClass()->containsProperty(propertyName);
}

template <typename SUPER, typename T>
QMap<QString, QSharedPointer<PropertyMap>> SingleTableInheritanceMap<SUPER, T>::getProperties() {
  auto properties = ClassMapBaseExt<T, QtOrm::Mapping::SubClassMap>::getProperties();
  auto superProperties = this->getSuperClass()->getProperties();
  for (auto key : superProperties.keys())
    properties[key] = superProperties.value(key);

  return properties;
}
}
}
#endif // SINGLETABLEINHERITANCEMAP_H
