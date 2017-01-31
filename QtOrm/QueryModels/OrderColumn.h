#ifndef ORDERCOLUMN_H
#define ORDERCOLUMN_H

#include <QString>

enum class Sort{
  ASC,
  DESC
};

struct OrderColumn{
  QString orderProperty;
  Sort sort;
};

#endif // ORDERCOLUMN_H
