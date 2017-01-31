#ifndef UPDATEFIELDQUERYMODEL_H
#define UPDATEFIELDQUERYMODEL_H

#include "UpdateQueryModel.h"

namespace QtOrm {
namespace Sql {

class UpdateFieldQueryModel : public UpdateQueryModel {
public:
  UpdateFieldQueryModel();
  ~UpdateFieldQueryModel();

  QString getPropertyName() const;
  void setPropertyName(const QString &value);

protected:
  virtual QSharedPointer<QueryTableModel> buildQueryTableModel() override;

protected:
  QString propertyName;
};
}
}
#endif // UPDATEFIELDQUERYMODEL_H
