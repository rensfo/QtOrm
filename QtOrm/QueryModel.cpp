#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

QueryModel::QueryModel(QObject *parent) : QObject(parent) {
}

QString QueryModel::getSqlText() {
  return sqlText;
}

QueryTableModel *QueryModel::getMainTableModel() const {
  return mainTableModel;
}

void QueryModel::setMainTableModel(QueryTableModel *value) {
  mainTableModel = value;
}

ClassMapBase *QueryModel::getClassBase() const {
  return classBase;
}

void QueryModel::setClassBase(ClassMapBase *value) {
  if (classBase != value) {
    classBase = value;
    buildModel();
  }
}
}
}
