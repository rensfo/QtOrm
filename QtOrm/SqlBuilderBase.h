#ifndef SQLMANAGERBASE_H
#define SQLMANAGERBASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextStream>
#include <QVariant>

#include "ConfigurateMap.h"

namespace QtOrm {
namespace Sql {
enum class SqlBuilderType { Simple, Setter };

class SqlBuilderBase : public QObject {
  Q_OBJECT
public:
  explicit SqlBuilderBase(const QSqlDatabase &database, QObject *parent = 0);
  QObject *getById(const QString &className, const QVariant &id);
  QList<QObject *> *getListObject(const QString &className, const QString property = QString(), const QVariant value = QVariant());
  virtual void insertObject(QObject &object) = 0;
  virtual void updateObject(const QObject &object) = 0;
  virtual void deleteObject(const QObject &object) = 0;

  QTextStream *getTextStream() const;
  void setTextStream(QTextStream *value);

protected:
  QString generateTableAlias();
  QString getCurrentTableAlias() const;
  void resetTableNumber();
  QString getPlaceHolder(const QString param);
  void sqlQueryToStream(QSqlQuery &query);
  QVariant prepareValue(QVariant &value);

private:
  QString getSelect() const;
  QString getFrom(const QString &tableName) const;
  QString getWhere(const QString &column, const QString &placeHolder) const;
  QList<QObject *> *getList(Mapping::ClassMapBase &classBase, QSqlQuery &query);
  void checkClass(const QString &className);
  void fillObject(const Mapping::ClassMapBase &classBase, const QSqlRecord &record, QObject &object);
  void fillOneToMany(const QMap<QString, Mapping::OneToMany *> &relations, const QString &idProperty, QObject &object);
  void fillOneToOne(Mapping::ClassMapBase &classBase, QObject &object);
  void objectSetProperty(QObject &object, const char *propertyName, const QVariant &value);

signals:

public slots:

protected:
  int tableNumber;
  const QString sqlQueryTemplate = "%1 %2 %3";
  const QString tableAliasTemplate = "tb_%1";
  QSqlDatabase database;
  QTextStream *textStream;
};
}
}
#endif // SQLMANAGERBASE_H
