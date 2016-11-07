#include "SqlBuilderBase.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QString>

#include "ClassMapBase.h"
#include "ConfigurationMap.h"
#include "SelectQueryModel.h"

namespace QtOrm {
namespace Sql {
using namespace Config;

SqlBuilderBase::SqlBuilderBase(QObject *parent) : QObject(parent) {
}

QSqlQuery SqlBuilderBase::selectQuery() {
  SelectQueryModel *selectQueryModel = new SelectQueryModel();
  selectQueryModel->setClassBase(classBase);
  selectQueryModel->setConditions(conditions);

  queryModel = selectQueryModel;

  QSqlQuery query(database);
  query.prepare(selectQueryModel->getSqlText());
  bindValues(query, conditions);

  return query;
}

QString SqlBuilderBase::getPlaceHolder(const QString param) {
  return QString(":%1").arg(param);
}

void SqlBuilderBase::bindValues(QSqlQuery &query, const GroupConditions &conditions) {
  for (Condition *f : conditions.getConditions()) {
    if (!f->getValues().isEmpty()) {
      QString columnName = classBase->getProperty(f->getPropertyName()).getColumn();
      query.bindValue(getPlaceHolder(columnName), f->getValues().first());
    }
  }
  for (GroupConditions *g : conditions.getGroups()) {
    bindValues(query, *g);
  }
}

QueryModel *SqlBuilderBase::getQueryModel() const
{
  return queryModel;
}

QSqlDatabase SqlBuilderBase::getDatabase() const {
  return database;
}

void SqlBuilderBase::setDatabase(const QSqlDatabase &value) {
  database = value;
}

QObject *SqlBuilderBase::getObject() const {
  return object;
}

void SqlBuilderBase::setObject(QObject *value) {
  object = value;
}

ClassMapBase *SqlBuilderBase::getClassBase() const {
  return classBase;
}

void SqlBuilderBase::setClassBase(ClassMapBase *value) {
  classBase = value;
}

GroupConditions SqlBuilderBase::getConditions() const {
  return conditions;
}

void SqlBuilderBase::setConditions(const GroupConditions &value) {
  conditions = value;
}
}
}
