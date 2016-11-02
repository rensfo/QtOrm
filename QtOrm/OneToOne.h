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
  QString getProperty() const;
  OneToOne &setProperty(const QString &property);

  QString getValueProperty() const;
  OneToOne &setValueProperty(const QString &column);

  QString getTableColumn() const;
  OneToOne &setTableColumn(const QString &value);

  OneToOne &setSaveCascade(bool value);

private:
  QString property;
  QString valueProperty;
  QString tableColumn;
};
}
}
#endif // ONETOONE_H
