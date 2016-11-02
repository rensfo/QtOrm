#ifndef SQLBUILDERBASE_H
#define SQLBUILDERBASE_H

#include <QHash>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextStream>
#include <QVariant>

#include "ConfigurationMap.h"
#include "Group.h"
#include "OneToOne.h"

namespace QtOrm {
namespace Sql {
enum class SqlBuilderType { Simple, Setter };

using namespace Mapping;

class SqlBuilderBase : public QObject {
  Q_OBJECT
public:
  explicit SqlBuilderBase(QObject *parent = nullptr);
  virtual QSqlQuery selectQuery();
  virtual QSqlQuery selectQuery(const QString &queryText);
  virtual QSqlQuery insertQuery() = 0;
  virtual QSqlQuery updateQuery() = 0;
  virtual QSqlQuery deleteQuery() = 0;
  virtual QSqlQuery updateOneColumnQuery(const QString &property) = 0;

  Group getConditions() const;
  void setConditions(const Group &value);

  QString getTableAlias() const;
  void setTableAlias(const QString &value);

  ClassMapBase *getClassBase() const;
  void setClassBase(ClassMapBase *value);

  static QString getTypeNameOfProperty(const QObject &obj, const QString &prop);
  static QString getTypeNameOfProperty(const QMetaObject &meta,
                                       const QString &prop);

  QObject *getObject() const;
  void setObject(QObject *value);

  QSqlDatabase getDatabase() const;
  void setDatabase(const QSqlDatabase &value);

  QMap<OneToOne *, QString> getOneToOneAlias() const;
  void setOneToOneAlias(const QMap<OneToOne *, QString> &value);

  QString generateTableAlias();

protected:
  void fillOneToOneAlias();
  void fillOneToOneAlias(const Mapping::ClassMapBase &classBase,
                         OneToOne *oneToOne);

  QString getCurrentTableAlias() const;
  void resetTableNumber();

  QString getPlaceHolder(const QString param);
  QVariant prepareValue(QVariant &value);
  virtual QString getLikeCondition(const QString &tableName,
                                   const QString &fieldName) const;

  QString getSelect(const Mapping::ClassMapBase &classBase);
  QString getOneToOneSelect(const Mapping::ClassMapBase &classBase,
                            OneToOne *oneToOne);
  QString getFrom(const Mapping::ClassMapBase &classBase);
  QString getOneToOneFrom(const Mapping::ClassMapBase &classBase,
                          OneToOne *oneToOne, const QString &mainTableAlias);
  QString getWhere(const QString &tableName, const Group &conditions) const;
  QString operationToString(const Condition &filter) const;
  QString groupOperationToString(GroupOperation groupOperation) const;
  void bindValues(QSqlQuery &query, const Group &conditions);

protected:
  int tableNumber;
  const QString tableAliasTemplate = "tb_%1";
  const QString sqlQueryTemplate = "%1 %2 %3";
  QSqlDatabase database;
  QMap<OneToOne *, QString> oneToOneAlias;
  ClassMapBase *classBase = nullptr;
  Group conditions;
  QString tableAlias;
  QObject *object;
};
}
}

Q_DECLARE_METATYPE(QObjectList)
#endif // SQLBUILDERBASE_H
