#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include "ClassMapBase.h"
#include "GroupConditions.h"
#include "QueryTableModel.h"

namespace QtOrm {
namespace Sql {

using Mapping::ClassMapBase;

enum class QueryModelType : int { Select, Insert, Update, Delete, UpdateColumn };

class QueryModel {
public:
  QueryModel();
  virtual ~QueryModel();
  virtual QString getSqlText();

  QSharedPointer<QueryTableModel> getMainTableModel() const;

  QSharedPointer<ClassMapBase> getClassBase() const;
  void setClassBase(QSharedPointer<ClassMapBase> &value);
  virtual void buildModel() = 0;

protected:
  void setMainTableModel(QSharedPointer<QueryTableModel> &value);

protected:
  QSharedPointer<QueryTableModel> mainTableModel;
  QString sqlText;
  QSharedPointer<ClassMapBase> classBase;
};
}
}

#endif // QUERYMODEL_H
