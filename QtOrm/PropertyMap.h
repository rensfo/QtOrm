#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <QString>

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

  QString getUpdateFunction() const;
  void setUpdateFunction(const QString &value);

private:
  QString name;
  QString column;
  bool isId;
};
}
}

#endif // PROPERTYMAP_H
