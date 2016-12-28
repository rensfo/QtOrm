#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

QueryModel::QueryModel(QObject *parent) : QObject(parent) {
}

QString QueryModel::getSqlText() {
  return sqlText;
}

QSharedPointer<QueryTableModel> QueryModel::getMainTableModel() const {
  return mainTableModel;
}

void QueryModel::setMainTableModel(QSharedPointer<QueryTableModel> &value) {
  mainTableModel = value;
}

QSharedPointer<ClassMapBase> QueryModel::getClassBase() const {
  return classBase;
}

void QueryModel::setClassBase(QSharedPointer<ClassMapBase> &value) {
  if (classBase != value) {
    classBase = value;
  }
}
}
}
