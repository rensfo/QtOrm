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
#include "UpdateQueryModel.h"

namespace QtOrm {
namespace Sql {
using namespace QtOrm::Config;

SimpleSqlBuilder::SimpleSqlBuilder(QObject *parent) : SqlBuilderBase(parent) {
}

QSqlQuery SimpleSqlBuilder::insertQuery() {
  InsertQueryModel *insertQueryModel = new InsertQueryModel();
  insertQueryModel->setHasLastInsertedIdFeature(hasLastInsertedIdFeature());
  insertQueryModel->setClassBase(classBase);

  queryModel = insertQueryModel;
  QString fullSqlText = insertQueryModel->getSqlText();

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindInsert(query);

  return query;
}

QSqlQuery SimpleSqlBuilder::updateQuery() {
  UpdateQueryModel *updateQueryModel = new UpdateQueryModel();
  updateQueryModel->setClassBase(classBase);

  queryModel = updateQueryModel;

  QString fullSqlText = updateQueryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindUpdate(query);

  return query;
}

QSqlQuery SimpleSqlBuilder::updateOneColumnQuery(const QString &property) {
  QString fullSqlText = getUpdateOneColumnText(property);
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindOneColumnUpdate(query, property);

  return query;
}

QSqlQuery SimpleSqlBuilder::deleteQuery() {
  DeleteQueryModel *deleteQueryModel = new DeleteQueryModel();
  deleteQueryModel->setClassBase(classBase);

  queryModel = deleteQueryModel;

  QString fullSqlText = deleteQueryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindDelete(query);

  return query;
}

QString SimpleSqlBuilder::getInsertText() {
  QStringList columns;
  QStringList values;
  for (auto prop : classBase->getProperties()) {
    if (prop->getIsId())
      continue;
    columns.append(prop->getColumn());
    values.append(getPlaceHolder(prop->getColumn()));
  }
  for (auto prop : classBase->getOneToOneRelations()) {
    columns.append(prop->getTableColumn());
    values.append(getPlaceHolder(prop->getTableColumn()));
  }
  QString fullSqlText;

  if (hasLastInsertedIdFeature())
    fullSqlText = QString("insert into %1(%2) values(%3)")
                      .arg(classBase->getTable())
                      .arg(columns.join(", "))
                      .arg(values.join(", "));
  else
    fullSqlText = QString("insert into %1(%2) values(%3) returning %4")
                      .arg(classBase->getTable())
                      .arg(columns.join(", "))
                      .arg(values.join(", "))
                      .arg(classBase->getIdProperty().getColumn());

  return fullSqlText;
}

QString SimpleSqlBuilder::getUpdateText() {
  QString setClause;
  QString whereClause;
  for (auto prop : classBase->getProperties()) {
    if (prop->getIsId())
      whereClause = QString("%1 = :%1").arg(prop->getColumn());
    else
      setClause += QString("%1%2 = :%2").arg(setClause.isEmpty() ? "" : ", ").arg(prop->getColumn());
  }
  for (auto prop : classBase->getOneToOneRelations()) {
    setClause += QString("%1%2 = :%2").arg(setClause.isEmpty() ? "" : ", ").arg(prop->getTableColumn());
  }

  QString fullSqlText = QString("update %1 set %2 where %3").arg(classBase->getTable()).arg(setClause).arg(whereClause);

  return fullSqlText;
}

QString SimpleSqlBuilder::getUpdateOneColumnText(const QString &propertyName) {
  QString tableColumnName = "";
  if (propertiesContains(propertyName)) {
    PropertyMap &property = classBase->getProperty(propertyName);
    tableColumnName = property.getColumn();
  } else {
    OneToOne *oneToOne = findOneToOneByPropertyName(propertyName);
    tableColumnName = oneToOne->getTableColumn();
  }

  QString setClause = QString("%1 = :%1").arg(tableColumnName);

  PropertyMap &idProperty = classBase->getIdProperty();
  QString whereClause = QString("%1 = :%1").arg(idProperty.getColumn());

  QString fullSqlText = QString("update %1 set %2 where %3").arg(classBase->getTable()).arg(setClause).arg(whereClause);

  return fullSqlText;
}

QString SimpleSqlBuilder::getDeleteText() {
  QString idColumnName = classBase->getIdProperty().getColumn();
  QString idPlaceHolder = getPlaceHolder(idColumnName);
  QString fullSqlText =
      QString("delete from %1 where %2 = %3").arg(classBase->getTable()).arg(idColumnName).arg(idPlaceHolder);

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
    QString refClassName = Mapping::ClassMapBase::getTypeNameOfProperty(*object, oneToOne->getProperty());
    Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(refClassName);
    QObject *objFromProp = refClassBase->getObjectByVariant(valFromProp);
    QVariant val;
    if (objFromProp) {
      QString propRefClass = refClassBase->getIdProperty().getName();
      val = objFromProp->property(propRefClass.toStdString().c_str());
    }
    query.bindValue(getPlaceHolder(oneToOne->getTableColumn()), val);
  }
}

void SimpleSqlBuilder::bindUpdate(QSqlQuery &query) {
  for (auto prop : classBase->getProperties()) {
    bind(query, *prop);
  }
  for (auto oneToOne : classBase->getOneToOneRelations()) {
    bind(query, *oneToOne);
  }
}

void SimpleSqlBuilder::bindOneColumnUpdate(QSqlQuery &query, const QString &propertyName) {
  PropertyMap &idProperty = classBase->getIdProperty();
  bind(query, idProperty);
  if (propertiesContains(propertyName)) {
    bind(query, classBase->getProperty(propertyName));
  } else {
    OneToOne *oneToOne = findOneToOneByPropertyName(propertyName);
    bind(query, *oneToOne);
  }
}

void SimpleSqlBuilder::bindDelete(QSqlQuery &query) {
  QVariant idPropertyValue = object->property(classBase->getIdProperty().getName().toStdString().c_str());
  QString idColumnName = classBase->getIdProperty().getColumn();
  QString idPlaceHolder = getPlaceHolder(idColumnName);
  query.bindValue(idPlaceHolder, idPropertyValue);
}

void SimpleSqlBuilder::bind(QSqlQuery &query, const PropertyMap &property) {
  QString placeHolder = getPlaceHolder(property.getColumn());
  QVariant propertyValue = object->property(property.getName().toStdString().c_str());
  query.bindValue(placeHolder, propertyValue);
}

void SimpleSqlBuilder::bind(QSqlQuery &query, const OneToOne &oneToOne) {
  QVariant valFromProp = object->property(oneToOne.getProperty().toStdString().c_str());
  QString refClassName = Mapping::ClassMapBase::getTypeNameOfProperty(*object, oneToOne.getProperty());
  Mapping::ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(refClassName);
  QObject *objFromProp = refClassBase->getObjectByVariant(valFromProp);
  QVariant valToQuery;
  if (objFromProp) {
    QString propRefClass = refClassBase->getIdProperty().getName();
    valToQuery = objFromProp->property(propRefClass.toStdString().c_str());
  }

  query.bindValue(getPlaceHolder(oneToOne.getTableColumn()), valToQuery);
}

bool SimpleSqlBuilder::propertiesContains(const QString &propertyName) {
  return classBase->getProperties().contains(propertyName);
}

OneToOne *SimpleSqlBuilder::findOneToOneByPropertyName(const QString &propertyName) {
  for (OneToOne *oneToOne : classBase->getOneToOneRelations()) {
    if (oneToOne->getProperty() == propertyName) {
      return oneToOne;
    }
  }

  return nullptr;
}

bool SimpleSqlBuilder::hasLastInsertedIdFeature() {
  return database.driver()->hasFeature(QSqlDriver::LastInsertId) && database.driverName() != "QPSQL";
}
}
}
