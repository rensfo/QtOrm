#include "SimpleSqlBuilder.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>

#include "Mappings/ConfigurationMap.h"
#include "QueryModels/DeleteQueryModel.h"
#include "QueryModels/InsertQueryModel.h"
#include "QueryModels/UpdateFieldQueryModel.h"
#include "QueryModels/UpdateQueryModel.h"
#include "Relations/OneToMany.h"

namespace QtOrm {
namespace Sql {
using namespace QtOrm::Config;

SimpleSqlBuilder::SimpleSqlBuilder(QObject *parent) : SqlBuilderBase(parent) {
}

SimpleSqlBuilder::SimpleSqlBuilder(const SimpleSqlBuilder &other) : SimpleSqlBuilder(other.parent()) {
  this->operator=(other);
}

QSqlQuery SimpleSqlBuilder::insertQuery() {
  using namespace std::placeholders;
  std::function<void(QSqlQuery &)> insertBindFunction = std::bind(&QtOrm::Sql::SimpleSqlBuilder::bindInsert, this, _1);
  return prepareSqlQuery(QueryModelType::Insert, insertBindFunction);
}

QSqlQuery SimpleSqlBuilder::updateQuery() {
  using namespace std::placeholders;
  std::function<void(QSqlQuery &)> updateBindFunction = std::bind(&QtOrm::Sql::SimpleSqlBuilder::bindUpdate, this, _1);
  return prepareSqlQuery(QueryModelType::Update, updateBindFunction);
}

QSqlQuery SimpleSqlBuilder::updateOneColumnQuery(const QString &propertyName) {
  this->propertyName = propertyName;

  using namespace std::placeholders;
  std::function<void(QSqlQuery &)> updateColumnBindFunction =
      std::bind(&QtOrm::Sql::SimpleSqlBuilder::bindOneColumnUpdate, this, _1);
  return prepareSqlQuery(QueryModelType::UpdateColumn, updateColumnBindFunction);
}

QSqlQuery SimpleSqlBuilder::deleteQuery() {
  queryModel = getQueryModel(QueryModelType::Delete);
  QSharedPointer<DeleteQueryModel> deleteQueryModel = queryModel.dynamicCast<DeleteQueryModel>();
  deleteQueryModel->buildModel();
  QString fullSqlText = deleteQueryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);

  bindDelete(query);

  return query;
}

SimpleSqlBuilder &SimpleSqlBuilder::operator=(const SimpleSqlBuilder &other) {
  database = other.getDatabase();
  classBase = other.getClassBase();
  queryModel = other.getQueryModel();

  return *this;
}

void SimpleSqlBuilder::bindInsert(QSqlQuery &query) {
  for (auto &prop : classBase->getProperties()) {
    if (prop->getIsId()) {
      if(!prop->getAutoincrement()) {
        if(classBase->isSubclass()){
          auto baseClass = classBase->toSubclass()->getBaseClass();
          bindQueryIdParam(query, prop, baseClass->getIdProperty());
        } else {
          bindQueryParam(query, prop);
        }
      }
    } else {
      bindQueryParam(query, prop);
    }
  }

  for (auto oneToOne : classBase->getOneToOneRelations()) {
    bindQueryParam(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindUpdate(QSqlQuery &query) {
  for (auto &prop : classBase->getProperties()) {
    bindQueryParam(query, prop);
  }

  for (auto &oneToOne : classBase->getOneToOneRelations()) {
    bindQueryParam(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindOneColumnUpdate(QSqlQuery &query) {
  QSharedPointer<PropertyMap> idProperty = classBase->getIdProperty();
  bindQueryParam(query, idProperty);
  if (classBase->containsProperty(propertyName)) {
    bindQueryParam(query, classBase->getProperty(propertyName));
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    bindQueryParam(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindDelete(QSqlQuery &query) {
  QVariant idPropertyValue = object->property(classBase->getIdProperty()->getName().toStdString().c_str());
  QString idColumnName = classBase->getIdProperty()->getColumn();
  QString idPlaceHolder = getPlaceHolder(idColumnName);
  query.bindValue(idPlaceHolder, idPropertyValue);
}

void SimpleSqlBuilder::bindQueryParam(QSqlQuery &query, QSharedPointer<PropertyMap> property) {
  QString placeHolder = getPlaceHolder(property->getColumn());
  QVariant propertyValue = object->property(property->getName().toStdString().c_str());
  QVariant resultValue;
  QVariant nullValue = property->getNull();
  if (propertyValue != nullValue) {
    resultValue = propertyValue;
  }
  query.bindValue(placeHolder, resultValue);
}

void SimpleSqlBuilder::bindQueryIdParam(QSqlQuery&query, QSharedPointer<PropertyMap> idProperty, QSharedPointer<PropertyMap> baseIdProperty)
{
  QString placeHolder = getPlaceHolder(idProperty->getColumn());
  QVariant resultValue = object->property(baseIdProperty->getName().toStdString().c_str());
  query.bindValue(placeHolder, resultValue);
}

void SimpleSqlBuilder::bindQueryParam(QSqlQuery &query, QSharedPointer<OneToOne> oneToOne) {
  QVariant valFromProp = object->property(oneToOne->getProperty().toStdString().c_str());
  QString refClassName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  QSharedPointer<ClassMapBase> refClassBase = configuration->getMappedClass(refClassName);

  QObject* objFromProp = valFromProp.value<QObject*>();
  QVariant valToQuery;
  if (objFromProp) {
    QString propRefClass = refClassBase->getIdProperty()->getName();
    valToQuery = objFromProp->property(propRefClass.toStdString().c_str());
  }

  query.bindValue(getPlaceHolder(oneToOne->getTableColumn()), valToQuery);
}

QSqlQuery SimpleSqlBuilder::prepareSqlQuery(QueryModelType modelType, std::function<void(QSqlQuery &)> bindFunction) {
  queryModel = getQueryModel(modelType);
  QString fullSqlText = queryModel->getSqlText();
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  bindFunction(query);

  return query;
}
}
}
