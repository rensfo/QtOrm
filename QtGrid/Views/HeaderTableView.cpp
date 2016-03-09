#include "HeaderTableView.h"
#include <QHeaderView>

HeaderTableView::HeaderTableView(QWidget *parent) : QTableView(parent) {
  tableModelHeader = new HeaderTableModel(this);
  setModel(tableModelHeader);
  filterDelegate = new HeaderTableDelegate(this);
  setItemDelegateForRow(0, filterDelegate);
  verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void HeaderTableView::hideColumns() {
  for (auto column : columns)
    setColumnHidden(column->getIndex(), !column->getVisible());
}

void HeaderTableView::showEvent(QShowEvent *event) {
  Q_UNUSED(event)

  hideColumns();
  openEditors();
}

QList<GridColumn *> HeaderTableView::getColumns() const {
  return columns;
}

void HeaderTableView::setColumns(const QList<GridColumn *> &value) {
  columns = value;
  hideColumns();
  openEditors();
}

void HeaderTableView::openEditors() {
  for (int i = 0; i < columns.count(); ++i)
    openPersistentEditor(model()->index(0, i));
}
