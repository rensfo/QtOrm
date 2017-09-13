#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include "Mappings/ClassMapBase.h"
#include "GroupConditions.h"
#include "QueryTableModel.h"
#include "Mappings/ConfigurationMap.h"

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

  QSharedPointer<Config::ConfigurationMap> getConfiguration() const;
  void setConfiguration(const QSharedPointer<Config::ConfigurationMap>&value);

protected:
  void setMainTableModel(QSharedPointer<QueryTableModel> &value);

protected:
  QSharedPointer<QueryTableModel> mainTableModel;
  QString sqlText;
  QSharedPointer<ClassMapBase> classBase;
  QSharedPointer<Config::ConfigurationMap> configuration;
};
}
}

#endif // QUERYMODEL_H
