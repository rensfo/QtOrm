#ifndef UPDATEFIELDQUERYMODEL_H
#define UPDATEFIELDQUERYMODEL_H

#include "UpdateQueryModel.h"

namespace QtOrm {
namespace Sql {

class UpdateFieldQueryModel : public UpdateQueryModel {
  Q_OBJECT
public:
  explicit UpdateFieldQueryModel(QObject *parent = nullptr);
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
