#ifndef HEADERTABLEVIEW_H
#define HEADERTABLEVIEW_H

#include "HeaderTableDelegate.h"
#include "GridColumn.h"
#include "HeaderTableModel.h"

#include <QObject>
#include <QTableView>

class HeaderTableView : public QTableView {
  Q_OBJECT

public:
  HeaderTableView(QWidget *parent = 0);

  void hideColumns();

  QList<GridColumn *> getColumns() const;
  void setColumns(const QList<GridColumn *> &value);
  void openEditors();

protected:
  void showEvent(QShowEvent *event);

private:
  QList<GridColumn *> columns;
  HeaderTableDelegate *filterDelegate;
  HeaderTableModel *tableModelHeader;
};

#endif // HEADERTABLEVIEW_H
