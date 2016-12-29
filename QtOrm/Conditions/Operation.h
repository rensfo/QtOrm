#ifndef OPERATION_H
#define OPERATION_H

#include <QMap>

namespace QtOrm {
namespace Sql {

enum class Operation : int {
  Like,
  Equal,
  NotEqual,
  IsNull,
  IsNotNull,
  In,
  Between,
  Greater,
  GreaterOrEqual,
  Less,
  LessOrEqual
};

}
}
#endif // OPERATION_H
