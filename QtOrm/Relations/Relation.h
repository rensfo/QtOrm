#ifndef RELATION_H
#define RELATION_H

#include <QObject>

namespace QtOrm {
namespace Mapping {

class Relation : public QObject {
  Q_OBJECT
public:
  explicit Relation(QObject *parent = nullptr);

  bool getSaveCascade() const;
  bool getDeleteCascade() const;

protected:
  bool saveCascade = false;
  bool deleteCascade = false;
};
}
}
#endif // RELATION_H
