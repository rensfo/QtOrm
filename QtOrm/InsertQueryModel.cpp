#include "InsertQueryModel.h"
#include "OneToOne.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Mapping::OneToOne;

InsertQueryModel::InsertQueryModel(QObject *parent) : QueryModel(parent) {
}

void InsertQueryModel::buildModel() {
  mainTableModel = buildQueryTableModel();
  sqlText = buildSql();
}

QueryTableModel *InsertQueryModel::buildQueryTableModel() {
  QueryTableModel *queryTableModel = new QueryTableModel();
  queryTableModel->setName(classBase->getTable());
  for (auto property : classBase->getProperties()) {
    if (property->getIsId()) {
      idColumn = property->getColumn();
      continue;
    }

    queryTableModel->addColumn(property->getColumn());
  }

  for(OneToOne *oneToOne : classBase->getOneToOneRelations())
    queryTableModel->addColumn(oneToOne->getTableColumn());

  return queryTableModel;
}

QString InsertQueryModel::buildSql() {
  QString columns, placeholders;
  for (QString column : mainTableModel->getColumns()) {
    if (columns.isEmpty())
      columns += column;
    else
      columns += ", " + column;

    if (placeholders.isEmpty())
      placeholders += ":" + column;
    else
      placeholders += ", :" + column;
  }

  if(hasLastInsertedIdFeature)
    return insertTemplateWithLastInsertedId.arg(mainTableModel->getName()).arg(columns).arg(placeholders);

  return insertTemplate.arg(mainTableModel->getName()).arg(columns).arg(placeholders).arg(idColumn);
}

bool InsertQueryModel::getHasLastInsertedIdFeature() const
{
    return hasLastInsertedIdFeature;
}

void InsertQueryModel::setHasLastInsertedIdFeature(bool value)
{
    hasLastInsertedIdFeature = value;
}
}
}
