#ifndef UPDATEQUERYMODEL_H
#define UPDATEQUERYMODEL_H

#include <QObject>

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class UpdateQueryModel : public QueryModel {
public:
  explicit UpdateQueryModel(QObject *parent = nullptr);
  virtual void buildModel() override;

protected:
  QueryTableModel *buildQueryTableModel();
  QString buildSql();

protected:
  const QString updateTemplate = "update %1 set %2 where %3 = :%3";
  QString idColumn;
};
}
}
#endif // UPDATEQUERYMODEL_H
