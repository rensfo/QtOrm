#ifndef SIMPLESQLMANAGER_H
#define SIMPLESQLMANAGER_H

#include <QSharedPointer>

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {
class SimpleSqlBuilder : public SqlBuilderBase {
  Q_OBJECT
public:
  SimpleSqlBuilder(QObject *parent = nullptr);
  virtual QSqlQuery insertQuery() override;
  virtual QSqlQuery updateQuery() override;
  virtual QSqlQuery updateOneColumnQuery(const QString &property) override;
  virtual QSqlQuery deleteQuery() override;

protected:
  virtual QString getUpdateOneColumnText(const QString &propertyName);
  virtual void bindInsert(QSqlQuery &query);
  virtual void bindUpdate(QSqlQuery &query);
  virtual void bindOneColumnUpdate(QSqlQuery &query, const QString &propertyName);
  virtual void bindDelete(QSqlQuery &query);
  void bind(QSqlQuery &query, QSharedPointer<PropertyMap> property);
  void bind(QSqlQuery &query, QSharedPointer<OneToOne> oneToOne);
  bool hasLastInsertedIdFeature();
};
}
}

#endif // SIMPLESQLMANAGER_H
