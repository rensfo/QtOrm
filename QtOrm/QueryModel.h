#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include <QObject>

#include "ClassMapBase.h"
#include "GroupConditions.h"
#include "QueryTableModel.h"

namespace QtOrm {
namespace Sql {

using Mapping::ClassMapBase;

class QueryModel : public QObject {
  Q_OBJECT
public:
  explicit QueryModel(QObject *parent = nullptr);
  virtual QString getSqlText();

  QueryTableModel *getMainTableModel() const;

  ClassMapBase *getClassBase() const;
  void setClassBase(ClassMapBase *value);

protected:
  void setMainTableModel(QueryTableModel *value);
  virtual void buildModel() = 0;
  QueryTableModel *buildQueryTableModel(ClassMapBase *classBase);

protected:
  QueryTableModel *mainTableModel;
  QString sqlText;
  ClassMapBase *classBase;
};
}
}

#endif // QUERYMODEL_H
