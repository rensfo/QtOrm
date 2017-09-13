#ifndef QUERYJOIN_H
#define QUERYJOIN_H

#include <QObject>
#include <QSharedPointer>

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

  QSharedPointer<QueryTableModel> getQueryTableModel() const;
  void setQueryTableModel(QSharedPointer<QueryTableModel> value);

protected:
  JoinType type;
  QString leftTableColumnName;
  QString rigthTableColumnName;
  QSharedPointer<QueryTableModel> queryTableModel;
};
}
}

#endif // QUERYJOIN_H
