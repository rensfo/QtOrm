#include "UpdateQueryModel.h"

#include "Relations/OneToOne.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Mapping::OneToOne;

UpdateQueryModel::UpdateQueryModel() : QueryModel() {
}

UpdateQueryModel::~UpdateQueryModel() {
}

void UpdateQueryModel::buildModel() {
  mainTableModel = buildQueryTableModel();
  sqlText = buildSql();
}

QSharedPointer<QueryTableModel> UpdateQueryModel::buildQueryTableModel() {
  QSharedPointer<QueryTableModel> queryTableModel = QSharedPointer<QueryTableModel>::create();
  queryTableModel->setName(classBase->getTable());

  idColumn = classBase->getIdColumn();
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

QString UpdateQueryModel::buildSql() {
  QStringList setClause;
  for (QString column : mainTableModel->getColumns()) {
    setClause << QString("%1 = :%1").arg(column);
  }

  return updateTemplate.arg(mainTableModel->getName()).arg(setClause.join(", ")).arg(idColumn);
}
}
}
