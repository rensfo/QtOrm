#include "DeleteQueryModel.h"

namespace QtOrm {
namespace Sql {

DeleteQueryModel::DeleteQueryModel(QObject *parent) : QueryModel(parent)
{

}

void DeleteQueryModel::buildModel()
{
  mainTableModel = buildQueryTableModel();
  sqlText = buildSql();
}

QueryTableModel *DeleteQueryModel::buildQueryTableModel()
{
  QueryTableModel *queryTableModel = new QueryTableModel();
  queryTableModel->setName(classBase->getTable());
  for (auto property : classBase->getProperties()) {
    if (property->getIsId()) {
      idColumn = property->getColumn();
      break;
    }
  }

  return queryTableModel;
}

QString DeleteQueryModel::buildSql()
{
  return deleteTemplate.arg(mainTableModel->getName()).arg(idColumn);
}
}
}
