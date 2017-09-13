#ifndef ONETOONE_H
#define ONETOONE_H

#include "Relation.h"

#include <QString>

namespace QtOrm {
namespace Mapping {

class OneToOne : public Relation {
  Q_OBJECT
public:
  explicit OneToOne(QObject *parent = nullptr);

  OneToOne &setProperty(const QString &property);

  QString getTableColumn() const;
  OneToOne &setTableColumn(const QString &value);

  OneToOne &setSaveCascade(bool value);

private:
  QString tableColumn;
};
}
}
#endif // ONETOONE_H
