#ifndef DELETEQUERYMODEL_H
#define DELETEQUERYMODEL_H

#include <QObject>

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class DeleteQueryModel : public QueryModel {
  Q_OBJECT
public:
  explicit DeleteQueryModel(QObject *parent = nullptr);

protected:
  virtual void buildModel() override;
  QueryTableModel *buildQueryTableModel();
  QString buildSql();

protected:
  QString idColumn;
  const QString deleteTemplate = "delete from %1 where %2 = :%2";
};
}
}
#endif // DELETEQUERYMODEL_H
