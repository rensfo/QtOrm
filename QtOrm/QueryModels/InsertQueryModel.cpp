#include "InsertQueryModel.h"
#include "Relations/OneToOne.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Mapping::OneToOne;

InsertQueryModel::InsertQueryModel() {
}

InsertQueryModel::~InsertQueryModel() {
}

void InsertQueryModel::buildModel() {
  mainTableModel = buildQueryTableModel();
  sqlText = buildSql();
}

QSharedPointer<QueryTableModel> InsertQueryModel::buildQueryTableModel() {
  QSharedPointer<QueryTableModel> queryTableModel = QSharedPointer<QueryTableModel>::create();
  queryTableModel->setName(classBase->getTable());
  for (auto property : classBase->getProperties()) {
    if (property->getIsId()) {
      idColumn = property->getColumn();
      continue;
    }

    queryTableModel->addColumn(property->getColumn());
  }

  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations())
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

  if (hasLastInsertedIdFeature)
    return insertTemplateWithLastInsertedId.arg(mainTableModel->getName()).arg(columns).arg(placeholders);

  return insertTemplate.arg(mainTableModel->getName()).arg(columns).arg(placeholders).arg(idColumn);
}

bool InsertQueryModel::getHasLastInsertedIdFeature() const {
  return hasLastInsertedIdFeature;
}

void InsertQueryModel::setHasLastInsertedIdFeature(bool value) {
  hasLastInsertedIdFeature = value;
}
}
}
