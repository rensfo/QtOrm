#ifndef QUERYCACHE_H
#define QUERYCACHE_H

#include <QObject>

#include "QueryModel.h"

namespace QtOrm
{

using Sql::QueryModel;

class QueryCache : public QObject
{
  Q_OBJECT

  struct CacheData
  {
    QueryModel *insertModel;
    QueryModel *updateModel;
    QueryModel *deleteModel;
    QMap<QString, QueryModel *> columnsUpdateModels;
  };

public:
  explicit QueryCache(QObject *parent = nullptr);

  void addInsertModel(const QString &className, QueryModel *model);
  void addUpdateModel(const QString &className, QueryModel *model);
  void addDeleteModel(const QString &className, QueryModel *model);
  void addColumnUpdateModel(const QString &className, const QString &column, QueryModel *model);

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
