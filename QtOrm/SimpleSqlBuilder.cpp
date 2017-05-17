#include "SimpleSqlBuilder.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>

#include "ConfigurationMap.h"
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
  queryCache = other.getQueryCache();

  return *this;
}

void SimpleSqlBuilder::bindInsert(QSqlQuery &query) {
  for (auto &prop : classBase->getProperties()) {
    if (prop->getIsId())
      continue;

    bindQueryParams(query, prop);
  }

  for (auto oneToOne : classBase->getOneToOneRelations()) {
    bindQueryParams(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindUpdate(QSqlQuery &query) {
  for (auto &prop : classBase->getProperties()) {
    bindQueryParams(query, prop);
  }

  for (auto &oneToOne : classBase->getOneToOneRelations()) {
    bindQueryParams(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindOneColumnUpdate(QSqlQuery &query) {
  QSharedPointer<PropertyMap> idProperty = classBase->getIdProperty();
  bindQueryParams(query, idProperty);
  if (classBase->containsProperty(propertyName)) {
    bindQueryParams(query, classBase->getProperty(propertyName));
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    bindQueryParams(query, oneToOne);
  }
}

void SimpleSqlBuilder::bindDelete(QSqlQuery &query) {
  QVariant idPropertyValue = object->property(classBase->getIdProperty()->getName().toStdString().c_str());
  QString idColumnName = classBase->getIdProperty()->getColumn();
  QString idPlaceHolder = getPlaceHolder(idColumnName);
  query.bindValue(idPlaceHolder, idPropertyValue);
}

void SimpleSqlBuilder::bindQueryParams(QSqlQuery &query, QSharedPointer<PropertyMap> property) {
  QString placeHolder = getPlaceHolder(property->getColumn());
  QVariant propertyValue = object->property(property->getName().toStdString().c_str());
  QVariant resultValue;
  if (propertyValue != property->getNull()) {
    resultValue = propertyValue;
  }
  query.bindValue(placeHolder, resultValue);
}

void SimpleSqlBuilder::bindQueryParams(QSqlQuery &query, QSharedPointer<OneToOne> oneToOne) {
  QVariant valFromProp = object->property(oneToOne->getProperty().toStdString().c_str());
  QString refClassName = Mapping::ClassMapBase::getTypeNameOfProperty(object, oneToOne->getProperty());
  QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClassName);
  QSharedPointer<QObject> objFromProp = refClassBase->castToQObjectSharedPointer(valFromProp);
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
