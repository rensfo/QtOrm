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
  idColumn = classBase->getColumnIdProperty();
  for (auto property : classBase->getProperties()) {
    if (property->getIsId()) {
      continue;
    }

    queryTableModel->addColumn(property->getColumn());
  }

  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations()){
    QString oneToOneColumn = oneToOne->getTableColumn();
    if(!queryTableModel->getColumns().contains(oneToOneColumn)){
      queryTableModel->addColumn(oneToOneColumn);
    }
  }

  return queryTableModel;
}

QString InsertQueryModel::buildSql() {
  QStringList columns, placeholders;
  for (QString column : mainTableModel->getColumns()) {
    columns << column;
    placeholders << ":" + column;
  }

  if (hasLastInsertedIdFeature)
    return insertTemplateWithLastInsertedId.arg(mainTableModel->getName()).arg(columns.join(", ")).arg(placeholders.join(", "));

  return insertTemplate.arg(mainTableModel->getName()).arg(columns.join(", ")).arg(placeholders.join(", ")).arg(idColumn);
}

bool InsertQueryModel::getHasLastInsertedIdFeature() const {
  return hasLastInsertedIdFeature;
}

void InsertQueryModel::setHasLastInsertedIdFeature(bool value) {
  hasLastInsertedIdFeature = value;
}
}
}
