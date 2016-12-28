#include "UpdateFieldQueryModel.h"

#include "OneToOne.h"
#include "PropertyMap.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Mapping::OneToOne;
using QtOrm::Mapping::PropertyMap;

UpdateFieldQueryModel::UpdateFieldQueryModel(QObject *parent) : UpdateQueryModel(parent) {
}

QtOrm::Sql::UpdateFieldQueryModel::~UpdateFieldQueryModel() {
}

QString UpdateFieldQueryModel::getPropertyName() const {
  return propertyName;
}

void UpdateFieldQueryModel::setPropertyName(const QString &value) {
  propertyName = value;
}

QSharedPointer<QueryTableModel> UpdateFieldQueryModel::buildQueryTableModel() {
  QSharedPointer<QueryTableModel> queryTableModel = QSharedPointer<QueryTableModel>::create();
  queryTableModel->setName(classBase->getTable());
  for (auto property : classBase->getProperties()) {
    if (property->getIsId()) {
      idColumn = property->getColumn();
      continue;
    }
  }

  QString tableColumnName = "";
  if (classBase->propertiesContains(propertyName)) {
    QSharedPointer<PropertyMap> property = classBase->getProperty(propertyName);
    tableColumnName = property->getColumn();
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    tableColumnName = oneToOne->getTableColumn();
  }

  queryTableModel->addColumn(tableColumnName);

  for (QSharedPointer<OneToOne> oneToOne : classBase->getOneToOneRelations())
    queryTableModel->addColumn(oneToOne->getTableColumn());

  return queryTableModel;
}
}
}
