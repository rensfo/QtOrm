#ifndef ONETOONE_H
#define ONETOONE_H

#include <QString>

namespace QtOrm {
namespace Mapping {

class OneToOne {
public:
  OneToOne();
  QString getProperty() const;
  OneToOne &setProperty(const QString &property);

  QString getValueProperty() const;
  OneToOne &setValueProperty(const QString &column);

  QString getTableColumn() const;
  void setTableColumn(const QString &value);

private:
  QString property;
  QString valueProperty;
  QString tableColumn;
};
}
}
#endif // ONETOONE_H
