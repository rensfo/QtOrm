#ifndef HEADERTABLEMODEL_H
#define HEADERTABLEMODEL_H

#include <QAbstractTableModel>

#include "GridColumn.h"
#include "Group.h"

class HeaderTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  HeaderTableModel(QObject *parent = 0);
  int rowCount(const QModelIndex &index) const;
  int columnCount(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

  QList<GridColumn *> getColumns() const;
  void setColumns(const QList<GridColumn *> &value);

private:
  void updateFilters();
  void updateOrCreateFilter(const QString &field, const QVariant &value);
  void removeFilter(const QString &field);

signals:
  void filtersChanged(Group *const);
  void filtersChanged(QMap<QString, QVariant>);

private:
  QList<GridColumn *> columns;
  QMap<QString, QVariant> filtersValues;
  Group *filters;
};

#endif // HEADERTABLEMODEL_H
