#ifndef STATICFILTER_H
#define STATICFILTER_H

#include "GridColumn.h"

#include <Group.h>
#include <QVariant>
#include <QWidget>

class StaticFilter : public QWidget {
  Q_OBJECT
public:
  explicit StaticFilter(QWidget *parent = 0);
  void setFilters(const QList<GridColumn *> &columns);
  QMap<QString, QVariant> getFilters() const;

private:
  void clearLayout();
  Group *converFilters();
signals:
  void filtersChanged(QMap<QString, QVariant>);
  void filtersChanged(Group *);

public slots:
  void filterItemChanged(QString fieldName, QVariant value);

private:
  QMap<QString, QVariant> filters;
  bool emitChangedSignal = false;
};

#endif // STATICFILTER_H
