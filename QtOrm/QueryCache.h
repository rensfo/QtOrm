#ifndef QUERYCACHE_H
#define QUERYCACHE_H

#include <QObject>
#include <QSharedPointer>

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
    QSharedPointer<QueryModel> selectModel;
    QSharedPointer<QueryModel> insertModel;
    QSharedPointer<QueryModel> updateModel;
    QSharedPointer<QueryModel> deleteModel;
    QMap<QString, QSharedPointer<QueryModel>> columnsUpdateModels;
  };

public:
  explicit QueryCache(QObject *parent = nullptr);

  void addModel(QueryModelType type, QSharedPointer<QueryModel> model, const QString &className, const QString &columnName);
  void addSelectModel(const QString &className, QSharedPointer<QueryModel> model);
  void addInsertModel(const QString &className, QSharedPointer<QueryModel> model);
  void addUpdateModel(const QString &className, QSharedPointer<QueryModel> model);
  void addDeleteModel(const QString &className, QSharedPointer<QueryModel> model);
  void addColumnUpdateModel(const QString &className, const QString &column, QSharedPointer<QueryModel> model);

  QSharedPointer<QueryModel> getModel(QueryModelType type, const QString &className, const QString &columnName = QString());
  QSharedPointer<QueryModel> getSelectModel(const QString &className);
  QSharedPointer<QueryModel> getInsertModel(const QString &className);
  QSharedPointer<QueryModel> getUpdateModel(const QString &className);
  QSharedPointer<QueryModel> getDeleteModel(const QString &className);
  QSharedPointer<QueryModel> getColumnUpdateModel(const QString &className, const QString &column);

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
