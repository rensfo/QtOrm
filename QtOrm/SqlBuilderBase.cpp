#include "SqlBuilderBase.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QString>

#include "ClassMapBase.h"
#include "ConfigurationMap.h"
#include "DeleteQueryModel.h"
#include "InsertQueryModel.h"
#include "SelectQueryModel.h"
#include "UpdateQueryModel.h"

namespace QtOrm {
namespace Sql {
using namespace Config;

SqlBuilderBase::SqlBuilderBase(QObject *parent) : QObject(parent) {
}

QSqlQuery SqlBuilderBase::selectQuery() {
  queryModel = getQueryModel(QueryModelType::Select);
  SelectQueryModel *selectQueryModel = dynamic_cast<SelectQueryModel *>(queryModel);
  selectQueryModel->buildModel();
  selectQueryModel->setConditions(conditions);

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
      QString columnName = classBase->getProperty(f->getPropertyName()).getColumn();
      QString placeHolder = placeHolders[f];
      query.bindValue(getPlaceHolder(placeHolder), f->getValues().first());
    }
  }
  for (GroupConditions *g : conditions.getGroups()) {
    bindValues(query, *g, placeHolders);
  }
}

QueryModel *SqlBuilderBase::getQueryModel(QueryModelType queryType, const QString &columnName)
{
  QString className = classBase->getClassName();
  if(queryCache)
  {
    if(QueryModel *model = queryCache->getModel(queryType, className, columnName))
    {
      return model;
    }
    else
    {
      return createModelAndAddToCache(queryType, className, columnName);
    }
  }

  return createModelAndAddToCache(queryType, className, columnName);
}

QueryModel *SqlBuilderBase::createModelAndAddToCache(QueryModelType queryType, const QString &className, const QString &columnName)
{
  QueryModel *newModel = createModel(queryType);
  if(queryCache)
  {
    queryCache->addModel(queryType, newModel, className, columnName);
  }

  return newModel;
}

QueryModel *SqlBuilderBase::createModel(QueryModelType queryType)
{
  QueryModel *result = nullptr;

  switch (queryType)
  {
    case QueryModelType::Select:
      result = new SelectQueryModel();
    break;
    case QueryModelType::Insert:
      result = new InsertQueryModel();
    break;
    case QueryModelType::Update:
      result = new UpdateQueryModel();
    break;
    case QueryModelType::UpdateColumn:
      throw std::string("update column not defined");
    break;
    case QueryModelType::Delete:
      result = new DeleteQueryModel();
    break;
    //throw exception
  }
  result->setClassBase(classBase);

  return result;
}

QueryCache *SqlBuilderBase::getQueryCache() const
{
  return queryCache;
}

void SqlBuilderBase::setQueryCache(QueryCache *value)
{
  queryCache = value;
}

QueryModel *SqlBuilderBase::getQueryModel() const
{
  return queryModel;
}

QSqlDatabase SqlBuilderBase::getDatabase() const {
  return database;
}

void SqlBuilderBase::setDatabase(const QSqlDatabase &value) {
  database = value;
}

QObject *SqlBuilderBase::getObject() const {
  return object;
}

void SqlBuilderBase::setObject(QObject *value) {
  object = value;
}

ClassMapBase *SqlBuilderBase::getClassBase() const {
  return classBase;
}

void SqlBuilderBase::setClassBase(ClassMapBase *value) {
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
