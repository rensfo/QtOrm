#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <QString>
#include <QVariant>

namespace QtOrm {
namespace Mapping {

class PropertyMap {
public:
  PropertyMap();
  PropertyMap(QString name);
  PropertyMap(QString name, QString column);

  PropertyMap &setName(QString name);
  QString getName() const;

  PropertyMap &setColumn(QString column);
  QString getColumn() const;

  PropertyMap &setIsId(bool isId);
  bool getIsId();

  bool getIsDiscriminator() const;
  PropertyMap &setIsDiscriminator(bool value);

  QVariant getNull() const;
  PropertyMap &setNull(const QVariant &value);

  bool getAutoincrement() const;
  PropertyMap &setAutoincrement(bool value);

private:
  QString name;
  QString column;
  bool isId = false;
  bool isDiscriminator = false;
  QVariant null;
  bool autoincrement = true;
};
}
}

#endif // PROPERTYMAP_H
