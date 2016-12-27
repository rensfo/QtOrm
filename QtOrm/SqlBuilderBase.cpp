#include "SqlBuilderBase.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QString>

#include "ClassMapBase.h"
#include "ConfigurationMap.h"
#include "DeleteQueryModel.h"
#include "InsertQueryModel.h"
#include "SelectQueryModel.h"
#include "UpdateFieldQueryModel.h"
#include "UpdateQueryModel.h"

#include <QDebug>

namespace QtOrm {
namespace Sql {
using namespace Config;

SqlBuilderBase::SqlBuilderBase(QObject *parent) : QObject(parent) {
}

QSqlQuery SqlBuilderBase::selectQuery() {
  queryModel = getQueryModel(QueryModelType::Select);
  QSharedPointer<SelectQueryModel> selectQueryModel = queryModel.objectCast<SelectQueryModel>();
  selectQueryModel->buildModel();
  selectQueryModel->setConditions(conditions);
  selectQueryModel->buildModel();

  QSqlQuery query(database);
  query.prepare(selectQueryModel->getSqlText());
  bindValues(query, conditions, selectQueryModel->getConditionPlaceholder());

  selectQueryModel->clearPlaceHolders();

  return query;
}

QString SqlBuilderBase::getPlaceHolder(const QString param) {
  return QString(":%1").arg(param);
}

void SqlBuilderBase::bindValues(QSqlQuery &query, const GroupConditions &conditions, const QMap<Condition *, QString> &placeHolders) {
  for (Condition *f : conditions.getConditions()) {
    if (!f->getValues().isEmpty()) {
//      QString columnName = classBase->getProperty(f->getPropertyName()).getColumn();
      QString placeHolder = placeHolders[f];
      query.bindValue(getPlaceHolder(placeHolder), f->getValues().first());
    }
  }
  for (GroupConditions *g : conditions.getGroups()) {
    bindValues(query, *g, placeHolders);
  }
}

QSharedPointer<QueryModel> SqlBuilderBase::getQueryModel(QueryModelType queryType, const QString &columnName) {
  QString className = classBase->getClassName();
  if(queryCache) {
    if(QSharedPointer<QueryModel> model = queryCache->getModel(queryType, className, columnName)) {
      return model;
    } else {
      return createModelAndAddToCache(queryType, className, columnName);
    }
  }

  return createModelAndAddToCache(queryType, className, columnName);
}

QSharedPointer<QueryModel> SqlBuilderBase::createModelAndAddToCache(QueryModelType queryType, const QString &className, const QString &columnName)
{
  QSharedPointer<QueryModel> newModel = createModel(queryType);
  if(queryCache) {
    queryCache->addModel(queryType, newModel, className, columnName);
  }

  return newModel;
}

QSharedPointer<QueryModel> SqlBuilderBase::createModel(QueryModelType queryType)
{
  QSharedPointer<QueryModel> result;

  switch (queryType)
  {
    case QueryModelType::Select:
      result = QSharedPointer<SelectQueryModel>::create();
    break;
    case QueryModelType::Insert:
      result = QSharedPointer<InsertQueryModel>::create();
    break;
    case QueryModelType::Update:
      result = QSharedPointer<UpdateQueryModel>::create();
    break;
    case QueryModelType::UpdateColumn:
      result = QSharedPointer<UpdateFieldQueryModel>::create();
    break;
    case QueryModelType::Delete:
      result = QSharedPointer<DeleteQueryModel>::create();
    break;
    //throw exception
  }
  result->setClassBase(classBase);

  return result;
}

QSharedPointer<QueryCache> SqlBuilderBase::getQueryCache() const
{
  return queryCache;
}

void SqlBuilderBase::setQueryCache(QSharedPointer<QueryCache> value)
{
  queryCache = value;
}

QSharedPointer<QueryModel> SqlBuilderBase::getQueryModel() const
{
  return queryModel;
}

QSqlDatabase SqlBuilderBase::getDatabase() const {
  return database;
}

void SqlBuilderBase::setDatabase(const QSqlDatabase &value) {
  database = value;
}

QSharedPointer<QObject> SqlBuilderBase::getObject() const {
  return object;
}

void SqlBuilderBase::setObject(QSharedPointer<QObject> value) {
  object = value;
}

QSharedPointer<ClassMapBase> SqlBuilderBase::getClassBase() const {
  return classBase;
}

void SqlBuilderBase::setClassBase(QSharedPointer<ClassMapBase> value) {
  classBase = value;
}

GroupConditions SqlBuilderBase::getConditions() const {
  return conditions;
}

void SqlBuilderBase::setConditions(const GroupConditions &value) {
  conditions = value;
}
}
}
