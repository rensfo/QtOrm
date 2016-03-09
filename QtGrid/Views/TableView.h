#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

#include "UniversalDelegate.h"
#include "UniversalTableModel.h"

class TableView : public QTableView {
  Q_OBJECT

public:
  explicit TableView(QWidget *parent = 0);
  ~TableView();

  QList<GridColumn *> getColumns() const;
  void setColumns(const QList<GridColumn *> &value);

  Mode getMode() const;
  void setMode(const Mode &value);
  void hideColumns();

signals:
  void modeChanged(Mode);
  void initDelegate(QObject *const, GridColumn *const, QWidget *) const;
  void endEdit(const QModelIndex &index, GridColumn *const column, const QVariant value) const;

private:
  QList<GridColumn *> columns;
  Mode mode;
  UniversalDelegate *universalDelegate;

  // QWidget interface
protected:
  void showEvent(QShowEvent *event);

  // QWidget interface
protected:
  void resizeEvent(QResizeEvent *event);
};

#endif // TABLEVIEW_H
