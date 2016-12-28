#include "QueryCache.h"

namespace QtOrm {

QueryCache::QueryCache(QObject *parent) : QObject(parent) {
}

void QueryCache::addModel(QueryModelType type, QSharedPointer<QueryModel> model, const QString &className,
                          const QString &propertyName) {
  switch (type) {
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
    addColumnUpdateModel(className, propertyName, model);
    break;
  case QueryModelType::Delete:
    addDeleteModel(className, model);
    break;
    // thrown exception
  }
}

void QueryCache::addSelectModel(const QString &className, QSharedPointer<QueryModel> model) {
  createCacheIfNotExists(className);
  data[className].selectModel = model;
}

void QueryCache::addInsertModel(const QString &className, QSharedPointer<QueryModel> model) {
  createCacheIfNotExists(className);
  data[className].insertModel = model;
}

void QueryCache::addUpdateModel(const QString &className, QSharedPointer<QueryModel> model) {
  createCacheIfNotExists(className);
  data[className].updateModel = model;
}

void QueryCache::addDeleteModel(const QString &className, QSharedPointer<QueryModel> model) {
  createCacheIfNotExists(className);
  data[className].deleteModel = model;
}

void QueryCache::addColumnUpdateModel(const QString &className, const QString &propertyName,
                                      QSharedPointer<QueryModel> model) {
  createCacheIfNotExists(className);
  data[className].columnsUpdateModels[propertyName] = model;
}

QSharedPointer<QueryModel> QueryCache::getModel(QueryModelType type, const QString &className,
                                                const QString &propertyName) {
  QSharedPointer<QueryModel> result;
  switch (type) {
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
    result = getColumnUpdateModel(className, propertyName);
    break;
  case QueryModelType::Delete:
    result = getDeleteModel(className);
    break;
    // thrown exception
  }

  return result;
}

QSharedPointer<QueryModel> QueryCache::getSelectModel(const QString &className) {
  if (!existsClass(className))
    return QSharedPointer<QueryModel>();

  return data[className].selectModel;
}

QSharedPointer<QueryModel> QueryCache::getInsertModel(const QString &className) {
  if (!existsClass(className))
    return QSharedPointer<QueryModel>();

  return data[className].insertModel;
}

QSharedPointer<QueryModel> QueryCache::getUpdateModel(const QString &className) {
  if (!existsClass(className))
    return QSharedPointer<QueryModel>();

  return data[className].updateModel;
}

QSharedPointer<QueryModel> QueryCache::getDeleteModel(const QString &className) {
  if (!existsClass(className))
    return QSharedPointer<QueryModel>();

  return data[className].deleteModel;
}

QSharedPointer<QueryModel> QueryCache::getColumnUpdateModel(const QString &className, const QString &propertyName) {
  if (!existColumnInCache(className, propertyName))
    return QSharedPointer<QueryModel>();

  return data[className].columnsUpdateModels[propertyName];
}

void QueryCache::clear() {
  data.clear();
}

bool QueryCache::exists(QtOrm::Sql::QueryModelType type, const QString &className) {
  return !getModel(type, className).isNull();
}

bool QueryCache::exists(QtOrm::Sql::QueryModelType type, const QString &className, const QString &propertyName) {
  return !getModel(type, className, propertyName).isNull();
}

void QueryCache::createCacheIfNotExists(const QString &className) {
  if (!existsClass(className)) {
    data.insert(className, CacheData());
  }
}

bool QueryCache::existsClass(const QString &className) {
  return data.contains(className);
}

bool QueryCache::existColumnInCache(const QString &className, const QString &propertyName) {
  if (!existsClass(className) || !data[className].columnsUpdateModels.contains(propertyName))
    return false;

  return true;
}
}
