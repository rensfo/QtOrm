#include "UpdateQueryModel.h"

#include "OneToOne.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Mapping::OneToOne;

UpdateQueryModel::UpdateQueryModel(QObject *parent) : QueryModel(parent) {
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

QString UpdateQueryModel::buildSql() {
  QString setClause;
  for (QString column : mainTableModel->getColumns()) {
    if (setClause.isEmpty())
      setClause += QString("%1 = :%1").arg(column);
    else
      setClause += QString(", %1 = :%1").arg(column);
  }

  return updateTemplate.arg(mainTableModel->getName()).arg(setClause).arg(idColumn);
}
}
}
