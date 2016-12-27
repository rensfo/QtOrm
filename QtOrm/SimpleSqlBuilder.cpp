#include "SimpleSqlBuilder.h"

#include <QDebug>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>

#include "ConfigurationMap.h"
#include "DeleteQueryModel.h"
#include "InsertQueryModel.h"
#include "OneToMany.h"
#include "UpdateFieldQueryModel.h"
#include "UpdateQueryModel.h"

namespace QtOrm {
namespace Sql {
using namespace QtOrm::Config;

SimpleSqlBuilder::SimpleSqlBuilder(QObject *parent) : SqlBuilderBase(parent) {
}

QSqlQuery SimpleSqlBuilder::insertQuery() {
  queryModel = getQueryModel(QueryModelType::Insert);
  QSharedPointer<InsertQueryModel> insertQueryModel = queryModel.objectCast<InsertQueryModel>();
  insertQueryModel->setHasLastInsertedIdFeature(hasLastInsertedIdFeature());
  insertQueryModel->buildModel();

  QString fullSqlText = insertQueryModel->getSqlText();

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindInsert(query);

  return query;
}

QSqlQuery SimpleSqlBuilder::updateQuery() {
  queryModel = getQueryModel(QueryModelType::Update);
  QSharedPointer<UpdateQueryModel> updateQueryModel = queryModel.objectCast<UpdateQueryModel>();
  updateQueryModel->buildModel();

  QString fullSqlText = updateQueryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindUpdate(query);

  return query;
}

QSqlQuery SimpleSqlBuilder::updateOneColumnQuery(const QString &property) {
  queryModel = getQueryModel(QueryModelType::UpdateColumn, property);
  QSharedPointer<UpdateFieldQueryModel> updateFieldQueryModel = queryModel.objectCast<UpdateFieldQueryModel>();
  updateFieldQueryModel->setPropertyName(property);
  updateFieldQueryModel->buildModel();

  QString fullSqlText = updateFieldQueryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindOneColumnUpdate(query, property);

  return query;
}

QSqlQuery SimpleSqlBuilder::deleteQuery() {
  queryModel = getQueryModel(QueryModelType::Delete);
  QSharedPointer<DeleteQueryModel> deleteQueryModel = queryModel.objectCast<DeleteQueryModel>();
  deleteQueryModel->buildModel();

  QString fullSqlText = deleteQueryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindDelete(query);

  return query;
}

QString SimpleSqlBuilder::getUpdateOneColumnText(const QString &propertyName) {
  QString tableColumnName = "";
  if (classBase->propertiesContains(propertyName)) {
    QSharedPointer<PropertyMap> property = classBase->getProperty(propertyName);
    tableColumnName = property->getColumn();
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    tableColumnName = oneToOne->getTableColumn();
  }

  QString setClause = QString("%1 = :%1").arg(tableColumnName);

  QSharedPointer<PropertyMap> idProperty = classBase->getIdProperty();
  QString whereClause = QString("%1 = :%1").arg(idProperty->getColumn());

  QString fullSqlText = QString("update %1 set %2 where %3").arg(classBase->getTable()).arg(setClause).arg(whereClause);

  return fullSqlText;
}

void SimpleSqlBuilder::bindInsert(QSqlQuery &query) {
  for (auto prop : classBase->getProperties()) {
    if (prop->getIsId())
      continue;
    query.bindValue(getPlaceHolder(prop->getColumn()), object->property(prop->getName().toStdString().c_str()));
  }

  for (auto oneToOne : classBase->getOneToOneRelations()) {
    QVariant valFromProp = object->property(oneToOne->getProperty().toStdString().c_str());
    QString refClassName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClassName);
    QSharedPointer<QObject> objFromProp = refClassBase->getObjectByVariant(valFromProp);
    QVariant val;
    if (objFromProp) {
      QString propRefClass = refClassBase->getIdProperty()->getName();
      val = objFromProp->property(propRefClass.toStdString().c_str());
    }
    query.bindValue(getPlaceHolder(oneToOne->getTableColumn()), val);
  }
}

void SimpleSqlBuilder::bindUpdate(QSqlQuery &query) {
  for (QSharedPointer<PropertyMap> prop : classBase->getProperties()) {
    bind(query, prop);
  }

  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()) {
    bind(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindOneColumnUpdate(QSqlQuery &query, const QString &propertyName) {
  QSharedPointer<PropertyMap> idProperty = classBase->getIdProperty();
  bind(query, idProperty);
  if (classBase->propertiesContains(propertyName)) {
    bind(query, classBase->getProperty(propertyName));
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    bind(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindDelete(QSqlQuery &query) {
  QVariant idPropertyValue = object->property(classBase->getIdProperty()->getName().toStdString().c_str());
  QString idColumnName = classBase->getIdProperty()->getColumn();
  QString idPlaceHolder = getPlaceHolder(idColumnName);
  query.bindValue(idPlaceHolder, idPropertyValue);
}

void SimpleSqlBuilder::bind(QSqlQuery &query, QSharedPointer<PropertyMap> property) {
  QString placeHolder = getPlaceHolder(property->getColumn());
  QVariant propertyValue = object->property(property->getName().toStdString().c_str());
  query.bindValue(placeHolder, propertyValue);
}

void SimpleSqlBuilder::bind(QSqlQuery &query, QSharedPointer<OneToOne> oneToOne) {
  QVariant valFromProp = object->property(oneToOne->getProperty().toStdString().c_str());
  QString refClassName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClassName);
  QSharedPointer<QObject> objFromProp = refClassBase->getObjectByVariant(valFromProp);
  QVariant valToQuery;
  if (objFromProp) {
    QString propRefClass = refClassBase->getIdProperty()->getName();
    valToQuery = objFromProp->property(propRefClass.toStdString().c_str());
  }

  query.bindValue(getPlaceHolder(oneToOne->getTableColumn()), valToQuery);
}

bool SimpleSqlBuilder::hasLastInsertedIdFeature() {
  return database.driver()->hasFeature(QSqlDriver::LastInsertId) && database.driverName() != "QPSQL";
}
}
}
