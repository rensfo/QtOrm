#include "QueryJoin.h"

#include "QueryTableModel.h"

namespace QtOrm {
namespace Sql {

QueryJoin::QueryJoin(QObject *parent) : QObject(parent) {
}

JoinType QueryJoin::getType() const {
  return type;
}

void QueryJoin::setType(const JoinType &value) {
  type = value;
}

QString QueryJoin::getLeftTableColumnName() const {
  return leftTableColumnName;
}

void QueryJoin::setLeftTableColumnName(const QString &value) {
  leftTableColumnName = value;
}

QString QueryJoin::getRigthTableColumnName() const {
  return rigthTableColumnName;
}

void QueryJoin::setRigthTableColumnName(const QString &value) {
  rigthTableColumnName = value;
}

QueryTableModel *QueryJoin::getQueryTableModel() const {
  return queryTableModel;
}

void QueryJoin::setQueryTableModel(QueryTableModel *value) {
  queryTableModel = value;
}
}
}
