#ifndef DELETEQUERYMODEL_H
#define DELETEQUERYMODEL_H

#include <QObject>

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class DeleteQueryModel : public QueryModel
{
  Q_OBJECT
public:
  explicit DeleteQueryModel(QObject *parent = nullptr);
  virtual void buildModel() override;

protected:
  QueryTableModel *buildQueryTableModel();
  QString buildSql();

protected:
  const QString deleteTemplate = "delete from %1 where %2 = :%2";
  QString idColumn;
};
}
}
#endif // DELETEQUERYMODEL_H
