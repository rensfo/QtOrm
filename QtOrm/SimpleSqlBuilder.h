#ifndef SIMPLESQLMANAGER_H
#define SIMPLESQLMANAGER_H

#include "SqlBuilderBase.h"

namespace QtOrm {
namespace Sql {
class SimpleSqlBuilder : public SqlBuilderBase {
  Q_OBJECT

public:
  SimpleSqlBuilder(const QSqlDatabase &database, QObject *parent = 0);
  void insertObject(QObject &object);
  void updateObject(const QObject &object);
  void deleteObject(const QObject &object);

private:
  void executeQuery(QSqlQuery &query);
};
}
}

#endif // SIMPLESQLMANAGER_H
