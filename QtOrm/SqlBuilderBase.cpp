#include "SqlBuilderBase.h"
#include "ClassMapBase.h"

namespace QtOrm {
namespace Config {
extern QMap<QString, QtOrm::Mapping::ClassMapBase *> mappedClass;
}

namespace Sql {
SqlBuilderBase::SqlBuilderBase(const QSqlDatabase &database, QObject *parent) : QObject(parent), database(database) {
}

QSqlQuery SqlBuilderBase::getListObject(const QString &objectName) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(objectName);
  resetTableNumber();

  this->generateTableAlias();
  QString select = getSelect();
  QString from = getFrom(classBase->getTable());

  QString fullSqlText = sqlQueryTemplate.arg(select).arg(from).arg("");

  QSqlQuery *query = new QSqlQuery(database);
  query->prepare(fullSqlText);

  return *query;
}

QSqlQuery SqlBuilderBase::getListObject(const QString &objectName, const QString property, const QVariant value) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(objectName);
  resetTableNumber();
  QString tableAlias = this->generateTableAlias();
  QString select = getSelect();
  QString from = getFrom(classBase->getTable());
  QString column = classBase->getProperty(property).getColumn();
  QString placeHolder = ":" + column;
  QString where = getWhere(tableAlias, column, placeHolder);

  QString fullSqlText = sqlQueryTemplate.arg(select).arg(from).arg(where);

  QSqlQuery *query = new QSqlQuery(database);
  query->prepare(fullSqlText);
  query->bindValue(placeHolder, value);

  return *query;
}

QString SqlBuilderBase::generateTableAlias() {
  return tableAliasTemplate.arg(++tableNumber);
}

QString SqlBuilderBase::getCurrentTableAlias() const {
  return tableAliasTemplate.arg(tableNumber);
}

void SqlBuilderBase::resetTableNumber() {
    tableNumber = 0;
}

QString SqlBuilderBase::getPlaceHolder(const QString param){
    return QString(":%1").arg(param);
}

QString SqlBuilderBase::getSelect() const {
  return QString("select %1.*").arg(getCurrentTableAlias());
}

QString SqlBuilderBase::getFrom(const QString &tableName) const {
  return QString("from %1 %2").arg(tableName).arg(getCurrentTableAlias());
}

QString SqlBuilderBase::getWhere(const QString &tableAlias, const QString &column, const QString &placeHolder) const {
  return QString("where %1.%2 = %3").arg(tableAlias).arg(column).arg(placeHolder);
}
}
}
