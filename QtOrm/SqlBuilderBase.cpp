#include "SqlBuilderBase.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QString>
#include <QSqlDriver>

#include "ClassMapBase.h"
#include "ConfigurationMap.h"
#include "QueryModels/DeleteQueryModel.h"
#include "QueryModels/InsertQueryModel.h"
#include "QueryModels/SelectQueryModel.h"
#include "QueryModels/UpdateFieldQueryModel.h"
#include "QueryModels/UpdateQueryModel.h"
#include "Conditions/Condition.h"

namespace QtOrm {
namespace Sql {
using namespace Config;

SqlBuilderBase::SqlBuilderBase(QObject *parent) : QObject(parent) {
}

QSqlQuery SqlBuilderBase::selectQuery() {
  queryModel = getQueryModel(QueryModelType::Select);

  QSqlQuery query(database);
  query.prepare(queryModel->getSqlText());

  QSharedPointer<SelectQueryModel> selectQueryModel = queryModel.dynamicCast<SelectQueryModel>();
  QSharedPointer<GroupConditions> sharedConditions = QSharedPointer<GroupConditions>::create(selectQueryModel->getConditions());
  bindValues(query, sharedConditions, selectQueryModel->getConditionPlaceholder());
  selectQueryModel->clearPlaceHolders();

  return query;
}

QString SqlBuilderBase::getPlaceHolder(const QString &param) {
  return QString(":%1").arg(param);
}

void SqlBuilderBase::bindValues(QSqlQuery &query, const QSharedPointer<GroupConditions> &conditions,
                                const QMap<QSharedPointer<Condition>, QString> &placeHolders) {
  for (QSharedPointer<Condition> &condition : conditions->getConditions()) {
    if (!condition->getValues().isEmpty()) {
      QString placeHolder = placeHolders[condition];

      QList<Operation> operations{Operation::Between, Operation::In};
      //      bool betweenOrIn = condition.dynamicCast<ConditionBetween>() || condition.dynamicCast<ConditionIn>();

      bool betweenOrIn = operations.contains(condition->getOperation());

      if (betweenOrIn) {
        QString almostReadyPlaceHolder = getPlaceHolder(placeHolder);
        int i = 1;
        for (QVariant &value : condition->getValues()) {
          QString valuePlaceholder = QString("%1_%2").arg(almostReadyPlaceHolder).arg(i);
          query.bindValue(valuePlaceholder, value);
          i++;
        }
      } else {
        query.bindValue(getPlaceHolder(placeHolder), condition->getValues().first());
      }
    }
  }

  for (QSharedPointer<GroupConditions> &g : conditions->getGroups()) {
    bindValues(query, g, placeHolders);
  }
}

QSharedPointer<QueryModel> SqlBuilderBase::getQueryModel(QueryModelType queryType) {
  return createModelAndAddToCache(queryType);
}

QSharedPointer<QueryModel> SqlBuilderBase::createModelAndAddToCache(QueryModelType queryType) {
  QSharedPointer<QueryModel> newModel = createModel(queryType);
  return newModel;
}

QSharedPointer<QueryModel> SqlBuilderBase::createModel(QueryModelType queryType) {
  QSharedPointer<QueryModel> result;

  switch (queryType) {
  case QueryModelType::Select: {
    QSharedPointer<SelectQueryModel> selectModel = QSharedPointer<SelectQueryModel>::create();
    selectModel->setClassBase(classBase);
    selectModel->buildModel();
    selectModel->setConditions(conditions);
    selectModel->setOrderColumns(orderBy);
    result = selectModel;
    break;
  }
  case QueryModelType::Insert: {
    QSharedPointer<InsertQueryModel> insertModel = QSharedPointer<InsertQueryModel>::create();
    insertModel->setClassBase(classBase);
    insertModel->setHasLastInsertedIdFeature(hasLastInsertedIdFeature());
    insertModel->buildModel();
    result = insertModel;
    break;
  }
  case QueryModelType::Update: {
    QSharedPointer<UpdateQueryModel> updateModel = QSharedPointer<UpdateQueryModel>::create();
    updateModel->setClassBase(classBase);
    updateModel->buildModel();
    result = updateModel;
    break;
  }
  case QueryModelType::UpdateColumn: {
    QSharedPointer<UpdateFieldQueryModel> updateColumnModel = QSharedPointer<UpdateFieldQueryModel>::create();
    updateColumnModel->setClassBase(classBase);
    updateColumnModel->setPropertyName(propertyName);
    updateColumnModel->buildModel();
    result = updateColumnModel;
    break;
  }
  case QueryModelType::Delete: {
    QSharedPointer<DeleteQueryModel> deleteModel = QSharedPointer<DeleteQueryModel>::create();
    deleteModel->setClassBase(classBase);
    deleteModel->buildModel();
    result = deleteModel;
    break;
  }
    // throw exception
  }

  return result;
}

bool SqlBuilderBase::hasLastInsertedIdFeature() {
  return database.driver()->hasFeature(QSqlDriver::LastInsertId) && database.driverName() != "QPSQL";
}

QList<OrderColumn> SqlBuilderBase::getOrderBy() const {
  return orderBy;
}

void SqlBuilderBase::setOrderBy(const QList<OrderColumn> &value) {
  orderBy = value;
}

QSharedPointer<QueryModel> SqlBuilderBase::getQueryModel() const {
  return queryModel;
}

QSqlDatabase SqlBuilderBase::getDatabase() const {
  return database;
}

void SqlBuilderBase::setDatabase(const QSqlDatabase &value) {
  database = value;
}

QSharedPointer<QObject> SqlBuilderBase::getObject() const {
  return object;
}

void SqlBuilderBase::setObject(QSharedPointer<QObject> value) {
  object = value;
}

QSharedPointer<ClassMapBase> SqlBuilderBase::getClassBase() const {
  return classBase;
}

void SqlBuilderBase::setClassBase(QSharedPointer<ClassMapBase> value) {
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
