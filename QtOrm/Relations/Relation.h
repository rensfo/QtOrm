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

protected:
  bool saveCascade = false;
};
}
}
#endif // RELATION_H
