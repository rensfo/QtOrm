#ifndef INSERTQUERYMODEL_H
#define INSERTQUERYMODEL_H

#include "QueryModel.h"

namespace QtOrm {
namespace Sql {

class InsertQueryModel : public QueryModel {
  Q_OBJECT
public:
  explicit InsertQueryModel(QObject *parent = nullptr);

  bool getHasLastInsertedIdFeature() const;
  void setHasLastInsertedIdFeature(bool value);

protected:
  virtual void buildModel() override;
  QueryTableModel *buildQueryTableModel();
  QString buildSql();

protected:
  const QString insertTemplate = "insert into %1(%2) values(%3) returning %4";
  const QString insertTemplateWithLastInsertedId = "insert into %1(%2) values(%3)";
  QString idColumn;
  bool hasLastInsertedIdFeature = false;
};
}
}
#endif // INSERTQUERYMODEL_H
