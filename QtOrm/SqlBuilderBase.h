#ifndef SQLBUILDERBASE_H
#define SQLBUILDERBASE_H

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextStream>
#include <QVariant>

#include "ConfigurationMap.h"
#include "GroupConditions.h"
#include "OneToOne.h"
#include "QueryCache.h"
#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

using namespace Mapping;

class SqlBuilderBase : public QObject {
  Q_OBJECT

public:
  explicit SqlBuilderBase(QObject *parent = nullptr);
  virtual QSqlQuery selectQuery();
  virtual QSqlQuery insertQuery() = 0;
  virtual QSqlQuery updateQuery() = 0;
  virtual QSqlQuery deleteQuery() = 0;
  virtual QSqlQuery updateOneColumnQuery(const QString &property) = 0;

  GroupConditions getConditions() const;
  virtual void setConditions(const GroupConditions &value);

  QSharedPointer<ClassMapBase> getClassBase() const;
  virtual void setClassBase(QSharedPointer<ClassMapBase> value);

  QSharedPointer<QObject> getObject() const;
  void setObject(QSharedPointer<QObject> value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  QSharedPointer<QueryModel> getQueryModel() const;

  QSharedPointer<QueryCache> getQueryCache() const;
  void setQueryCache(QSharedPointer<QueryCache> value);

protected:
  QString getPlaceHolder(const QString param);
  void bindValues(QSqlQuery &query, const GroupConditions &conditions, const QMap<Condition *, QString> &placeHolders);
  QSharedPointer<QueryModel> getQueryModel(QueryModelType queryType, const QString &columnName = QString());
  QSharedPointer<QueryModel> createModelAndAddToCache(QueryModelType queryType, const QString &className, const QString &columnName = QString());
  QSharedPointer<QueryModel> createModel(QueryModelType queryType);

protected:
  QSqlDatabase database;
  QSharedPointer<ClassMapBase> classBase;
  GroupConditions conditions;
  QSharedPointer<QObject> object;
  QSharedPointer<QueryModel> queryModel;
  QSharedPointer<QueryCache> queryCache;
};
}
}

Q_DECLARE_METATYPE(QObjectList)
#endif // SQLBUILDERBASE_H
