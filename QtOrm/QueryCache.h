#ifndef QUERYCACHE_H
#define QUERYCACHE_H

#include <QObject>

#include "QueryModel.h"

namespace QtOrm
{

using Sql::QueryModel;
using Sql::QueryModelType;

class QueryCache : public QObject
{
  Q_OBJECT

  struct CacheData
  {
    QueryModel *selectModel;
    QueryModel *insertModel;
    QueryModel *updateModel;
    QueryModel *deleteModel;
    QMap<QString, QueryModel *> columnsUpdateModels;
  };

public:
  explicit QueryCache(QObject *parent = nullptr);

  void addModel(QueryModelType type, QueryModel *model, const QString &className, const QString &columnName);
  void addSelectModel(const QString &className, QueryModel *model);
  void addInsertModel(const QString &className, QueryModel *model);
  void addUpdateModel(const QString &className, QueryModel *model);
  void addDeleteModel(const QString &className, QueryModel *model);
  void addColumnUpdateModel(const QString &className, const QString &column, QueryModel *model);

  QueryModel *getModel(QueryModelType type, const QString &className, const QString &columnName = QString());
  QueryModel *getSelectModel(const QString &className);
  QueryModel *getInsertModel(const QString &className);
  QueryModel *getUpdateModel(const QString &className);
  QueryModel *getDeleteModel(const QString &className);
  QueryModel *getColumnUpdateModel(const QString &className, const QString &column);

  void clear();

protected:
  void createCacheIfNotExists(const QString &className);
  bool existsClassInCache(const QString &className);
  bool existColumnInCache(const QString &className, const QString &column);

protected:
  QMap<QString, CacheData> data;
};
}
#endif // QUERYCACHE_H
