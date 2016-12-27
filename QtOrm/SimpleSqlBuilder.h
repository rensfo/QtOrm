#ifndef SIMPLESQLMANAGER_H
#define SIMPLESQLMANAGER_H

#include <QSharedPointer>
#include <functional>

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {
class SimpleSqlBuilder : public SqlBuilderBase {
  Q_OBJECT
public:
  explicit SimpleSqlBuilder(QObject *parent = nullptr);
  SimpleSqlBuilder(const SimpleSqlBuilder &other);
  virtual QSqlQuery insertQuery() override;
  virtual QSqlQuery updateQuery() override;
  virtual QSqlQuery updateOneColumnQuery(const QString &propertyName) override;
  virtual QSqlQuery deleteQuery() override;
  SimpleSqlBuilder &operator=(const SimpleSqlBuilder &other);

protected:
  virtual void bindInsert(QSqlQuery &query);
  virtual void bindUpdate(QSqlQuery &query);
  virtual void bindOneColumnUpdate(QSqlQuery &query);
  virtual void bindDelete(QSqlQuery &query);
  void bindQueryParams(QSqlQuery &query, QSharedPointer<PropertyMap> property);
  void bindQueryParams(QSqlQuery &query, QSharedPointer<OneToOne> oneToOne);
  QSqlQuery prepareSqlQuery(QueryModelType modelType, std::function<void(QSqlQuery &)> bindFunction);

};
}
}

#endif // SIMPLESQLMANAGER_H
