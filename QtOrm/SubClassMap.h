#ifndef SUBCLASSMAP_H
#define SUBCLASSMAP_H

#include "ClassMapBase.h"

namespace QtOrm {
namespace Mapping {

enum class InheritanceType {
  None,
  SingleTable,
  ClassTable
};

class SubClassMap : public ClassMapBase {
  Q_OBJECT
public:
  explicit SubClassMap(QObject *parent = nullptr);

  QString getSuperClassName() const;
  QSharedPointer<ClassMapBase> getSuperClass() const;
  void setSuperClass(const QSharedPointer<ClassMapBase>&value);
  virtual bool isSubclass() override;

  InheritanceType getInheritanceType() const;

  QSharedPointer<ClassMapBase> getBaseClass() const;

  static bool isClassTableInheritance(const QSharedPointer<ClassMapBase>&classBase);

  QMap<QString, QSharedPointer<PropertyMap>> getAllProperties();

  QString getClassNameByProperty(const QString &property);

protected:
  void setInheritanceType(const InheritanceType&value);

private:
  QSharedPointer<ClassMapBase> superClass;
  InheritanceType inheritanceType = InheritanceType::None;
};

}
}

#endif // SUBCLASSMAP_H
