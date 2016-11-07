#ifndef QUERYJOIN_H
#define QUERYJOIN_H

#include <QObject>

namespace QtOrm {
namespace Sql {

class QueryTableModel;

enum class JoinType { Inner, Left, Right, FullOuter };

class QueryJoin : public QObject {
  Q_OBJECT
public:
  explicit QueryJoin(QObject *parent = nullptr);

  JoinType getType() const;
  void setType(const JoinType &value);

  QString getLeftTableColumnName() const;
  void setLeftTableColumnName(const QString &value);

  QString getRigthTableColumnName() const;
  void setRigthTableColumnName(const QString &value);

  QueryTableModel *getQueryTableModel() const;
  void setQueryTableModel(QueryTableModel *value);

protected:
  JoinType type;
  QString leftTableColumnName;
  QString rigthTableColumnName;
  QueryTableModel *queryTableModel = nullptr;
};
}
}

#endif // QUERYJOIN_H
