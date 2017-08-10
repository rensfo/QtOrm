#include "UpdateFieldQueryModel.h"

#include "Relations/OneToOne.h"
#include "Mappings/PropertyMap.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Mapping::OneToOne;
using QtOrm::Mapping::PropertyMap;

UpdateFieldQueryModel::UpdateFieldQueryModel() : UpdateQueryModel() {
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
  idColumn = classBase->getIdColumn();

  QString tableColumnName = "";
  if (classBase->containsProperty(propertyName)) {
    QSharedPointer<PropertyMap> property = classBase->getProperty(propertyName);
    tableColumnName = property->getColumn();
  } else {
    QSharedPointer<OneToOne> oneToOne = classBase->findOneToOneByPropertyName(propertyName);
    tableColumnName = oneToOne->getTableColumn();
  }

  queryTableModel->addColumn(tableColumnName);

  return queryTableModel;
}
}
}
