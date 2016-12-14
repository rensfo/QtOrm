#include "QueryCache.h"

namespace QtOrm
{

QueryCache::QueryCache(QObject *parent) : QObject(parent) {}

void QueryCache::addModel(QueryModelType type, QSharedPointer<QueryModel> model, const QString &className, const QString &columnName)
{
  switch (type)
  {
    case QueryModelType::Select:
      addSelectModel(className, model);
    break;
    case QueryModelType::Insert:
      addInsertModel(className, model);
    break;
    case QueryModelType::Update:
      addUpdateModel(className, model);
    break;
    case QueryModelType::UpdateColumn:
      addColumnUpdateModel(className, columnName, model);
    break;
    case QueryModelType::Delete:
      addDeleteModel(className, model);
    break;
    //thrown exception
  }
}

void QueryCache::addSelectModel(const QString &className, QSharedPointer<QueryModel> model)
{
  createCacheIfNotExists(className);
  data[className].selectModel = model;
}

void QueryCache::addInsertModel(const QString &className, QSharedPointer<QueryModel> model)
{
  createCacheIfNotExists(className);
  data[className].insertModel = model;
}

void QueryCache::addUpdateModel(const QString &className, QSharedPointer<QueryModel> model)
{
  createCacheIfNotExists(className);
  data[className].updateModel = model;
}

void QueryCache::addDeleteModel(const QString &className, QSharedPointer<QueryModel> model)
{
  createCacheIfNotExists(className);
  data[className].deleteModel = model;
}

void QueryCache::addColumnUpdateModel(const QString &className, const QString &column, QSharedPointer<QueryModel> model)
{
  createCacheIfNotExists(className);
  data[className].columnsUpdateModels[column] = model;
}

QSharedPointer<QueryModel> QueryCache::getModel(QueryModelType type, const QString &className, const QString &columnName)
{
  QSharedPointer<QueryModel> result;
  switch (type)
  {
    case QueryModelType::Select:
      result = getSelectModel(className);
    break;
    case QueryModelType::Insert:
      result = getInsertModel(className);
    break;
    case QueryModelType::Update:
      result = getUpdateModel(className);
    break;
    case QueryModelType::UpdateColumn:
      result = getColumnUpdateModel(className, columnName);
    break;
    case QueryModelType::Delete:
      result = getDeleteModel(className);
    break;
    //thrown exception
  }

  return result;
}

QSharedPointer<QueryModel> QueryCache::getSelectModel(const QString &className)
{
  if (!existsClassInCache(className))
    return QSharedPointer<QueryModel>();

  return data[className].selectModel;
}

QSharedPointer<QueryModel> QueryCache::getInsertModel(const QString &className)
{
  if (!existsClassInCache(className))
    return QSharedPointer<QueryModel>();

  return data[className].insertModel;
}

QSharedPointer<QueryModel> QueryCache::getUpdateModel(const QString &className)
{
  if (!existsClassInCache(className))
    return QSharedPointer<QueryModel>();

  return data[className].updateModel;
}

QSharedPointer<QueryModel> QueryCache::getDeleteModel(const QString &className)
{
  if (!existsClassInCache(className))
    return QSharedPointer<QueryModel>();

  return data[className].deleteModel;
}

QSharedPointer<QueryModel> QueryCache::getColumnUpdateModel(const QString &className, const QString &column)
{
  if (!existColumnInCache(className, column))
    return QSharedPointer<QueryModel>();

  return data[className].columnsUpdateModels[column];
}

void QueryCache::clear()
{
  data.clear();
}

void QueryCache::createCacheIfNotExists(const QString &className)
{
  if (!existsClassInCache(className))
  {
    data.insert(className, CacheData());
  }
}

bool QueryCache::existsClassInCache(const QString &className) { return data.contains(className); }

bool QueryCache::existColumnInCache(const QString &className, const QString &column)
{
  if (!existsClassInCache(className) || !data[className].columnsUpdateModels.contains(column))
    return false;

  return true;
}
}
