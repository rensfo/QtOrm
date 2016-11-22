#include "QueryCache.h"

namespace QtOrm
{

QueryCache::QueryCache(QObject *parent) : QObject(parent) {}

void QueryCache::addInsertModel(const QString &className, QtOrm::Sql::QueryModel *model)
{
  createCacheIfNotExists(className);
  data[className].insertModel = model;
}

void QueryCache::addUpdateModel(const QString &className, QtOrm::Sql::QueryModel *model)
{
  createCacheIfNotExists(className);
  data[className].updateModel = model;
}

void QueryCache::addDeleteModel(const QString &className, QtOrm::Sql::QueryModel *model)
{
  createCacheIfNotExists(className);
  data[className].deleteModel = model;
}

void QueryCache::addColumnUpdateModel(const QString &className, const QString &column, QtOrm::Sql::QueryModel *model)
{
  createCacheIfNotExists(className);
  data[className].columnsUpdateModels[column] = model;
}

QtOrm::Sql::QueryModel *QueryCache::getInsertModel(const QString &className)
{
  if (!existsClassInCache(className))
    return nullptr;

  return data[className].insertModel;
}

QtOrm::Sql::QueryModel *QueryCache::getUpdateModel(const QString &className)
{
  if (!existsClassInCache(className))
    return nullptr;

  return data[className].updateModel;
}

QtOrm::Sql::QueryModel *QueryCache::getDeleteModel(const QString &className)
{
  if (!existsClassInCache(className))
    return nullptr;

  return data[className].deleteModel;
}

QtOrm::Sql::QueryModel *QueryCache::getColumnUpdateModel(const QString &className, const QString &column)
{
  if (!existColumnInCache(className, column))
    return nullptr;

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
