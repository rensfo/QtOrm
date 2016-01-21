#ifndef SQLMANAGERBASE_H
#define SQLMANAGERBASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "ConfigurateMap.h"

namespace QtOrm {
namespace Sql {
enum class SqlBuilderType { Simple, Setter };

class SqlBuilderBase : public QObject {
  Q_OBJECT
public:
  explicit SqlBuilderBase(const QSqlDatabase &database, QObject *parent = 0);
  QSqlQuery getListObject(const QString &objectName);
  QSqlQuery getListObject(const QString &objectName, const QString property,
                          const QVariant value);
  virtual void insertObject(QObject &object) = 0;
  virtual void updateObject(const QObject &object) = 0;
  virtual void deleteObject(const QObject &object) = 0;

protected:
  QString generateTableAlias();
  QString getCurrentTableAlias() const;
  void resetTableNumber();
  QString getPlaceHolder(const QString param);

private:
  QString getSelect() const;
  QString getFrom(const QString &tableName) const;
  QString getWhere(const QString &tableAlias, const QString &column,
                   const QString &placeHolder) const;

signals:

public slots:

protected:
  int tableNumber;
  const QString sqlQueryTemplate = "%1 %2 %3";
  const QString tableAliasTemplate = "tb_%1";
  QSqlDatabase database;
};
}
}
#endif // SQLMANAGERBASE_H
