#ifndef UPDATEQUERYMODEL_H
#define UPDATEQUERYMODEL_H

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class UpdateQueryModel : public QueryModel {
public:
  UpdateQueryModel();
  ~UpdateQueryModel();
  virtual void buildModel() override;

protected:
  virtual QSharedPointer<QueryTableModel> buildQueryTableModel();
  QString buildSql();

protected:
  const QString updateTemplate = "update %1 set %2 where %3 = :%3";
  QString idColumn;
};
}
}
#endif // UPDATEQUERYMODEL_H
