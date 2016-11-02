#ifndef SIMPLESQLMANAGER_H
#define SIMPLESQLMANAGER_H

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
  //  void deleteChildren(QObject &object);

protected:
  virtual QString getInsertText();
  virtual QString getUpdateText();
  virtual QString getUpdateOneColumnText(const QString &propertyName);
  virtual QString getDeleteText();
  virtual void bindInsert(QSqlQuery &query);
  virtual void bindUpdate(QSqlQuery &query);
  virtual void bindOneColumnUpdate(QSqlQuery &query,
                                   const QString &propertyName);
  virtual void bindDelete(QSqlQuery &query);
  void bind(QSqlQuery &query, const PropertyMap &property);
  void bind(QSqlQuery &query, const OneToOne &oneToOne);
  bool propertiesContains(const QString &propertyName);
  OneToOne *findOneToOneByPropertyName(const QString &propertyName);
};
}
}

#endif // SIMPLESQLMANAGER_H
