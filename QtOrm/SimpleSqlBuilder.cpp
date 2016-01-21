#include "SimpleSqlBuilder.h"

#include <QSqlError>

namespace QtOrm {
namespace Sql {
SimpleSqlBuilder::SimpleSqlBuilder(const QSqlDatabase &database, QObject *parent) : SqlBuilderBase(database, parent) {
}

void SimpleSqlBuilder::insertObject(QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  QStringList columns;
  QStringList values;
  foreach (auto prop, classBase->getProperties()) {
    if (prop->getIsId())
      continue;
    columns.append(prop->getColumn());
    values.append(getPlaceHolder(prop->getColumn()));
  }
  QString fullSqlText = QString("insert into %1(%2) values(%3)")
                            .arg(classBase->getTable())
                            .arg(columns.join(", "))
                            .arg(values.join(", "));

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  foreach (auto prop, classBase->getProperties()) {
    if (prop->getIsId())
      continue;
    query.bindValue(getPlaceHolder(prop->getColumn()), object.property(prop->getName().toStdString().c_str()));
  }
  executeQuery(query);

  object.setProperty(classBase->getIdProperty().getName().toStdString().c_str(), query.lastInsertId());
}

void SimpleSqlBuilder::updateObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  QString setClause;
  QString whereClause;
  foreach (auto prop, classBase->getProperties()) {
    if (prop->getIsId())
      whereClause = QString("%1 = :%1").arg(prop->getColumn());
    else
      setClause += QString("%1%2 = :%2").arg(setClause.isEmpty() ? "" : ", ").arg(prop->getColumn());
  }

  QString fullSqlText = QString("update %1 set %2 where %3").arg(classBase->getTable()).arg(setClause).arg(whereClause);
  QSqlQuery query(database);
  query.prepare(fullSqlText);
  foreach (auto prop, classBase->getProperties()) {
    QString idPlaceHolder = getPlaceHolder(prop->getColumn());
    QVariant idValue = object.property(prop->getName().toStdString().c_str());
    query.bindValue(idPlaceHolder, idValue);
  }

  executeQuery(query);
}

void SimpleSqlBuilder::deleteObject(const QObject &object) {
  Mapping::ClassMapBase *classBase = Config::ConfigurateMap::getMappedClass(object.metaObject()->className());

  QString idColumnName = classBase->getIdProperty().getColumn();
  QString idPlaceHolder = getPlaceHolder(idColumnName);
  QString fullSqlText =
      QString("delete from %1 where %2 = %3").arg(classBase->getTable()).arg(idColumnName).arg(idPlaceHolder);

  QSqlQuery query(database);
  query.prepare(fullSqlText);
  QVariant idPropertyValue = object.property(classBase->getIdProperty().getName().toStdString().c_str());
  query.bindValue(idPlaceHolder, idPropertyValue);

  executeQuery(query);
}

void SimpleSqlBuilder::executeQuery(QSqlQuery &query) {
  if (!query.exec()) {
    throw new Exception(query.lastError().text());
  }
}
}
}
