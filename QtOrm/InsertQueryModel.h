#ifndef INSERTQUERYMODEL_H
#define INSERTQUERYMODEL_H

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class InsertQueryModel : public QueryModel {
  Q_OBJECT
public:
  explicit InsertQueryModel(QObject *parent = nullptr);

protected:
  virtual void buildModel() override;
  QueryTableModel *buildQueryTableModel();
  QString buildSql();

protected:
  const QString insertTemplate = "insert into %1(%2) values(%3) returning %4";
  QString idColumn;
};
}
}
#endif // INSERTQUERYMODEL_H
