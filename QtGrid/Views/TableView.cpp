#include "TableView.h"

#include <QLabel>
#include <QHeaderView>
#include <QDebug>

TableView::TableView(QWidget *parent) : QTableView(parent) {
  universalDelegate = new UniversalDelegate(this);
  setItemDelegate(universalDelegate);
  connect(universalDelegate,
          SIGNAL(initDelegate(QObject * const, GridColumn *const, QWidget *)),
          this,
          SIGNAL(initDelegate(QObject * const, GridColumn *const, QWidget *)));
  connect(universalDelegate,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)),
          this,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)));

  horizontalHeader()->setSectionsMovable(true);
}

TableView::~TableView() {
}

QList<GridColumn *> TableView::getColumns() const {
  return columns;
}

void TableView::setColumns(const QList<GridColumn *> &value) {
  columns = value;
  hideColumns();
}

Mode TableView::getMode() const {
  return mode;
}

void TableView::setMode(const Mode &value) {
  if (mode != value) {
    mode = value;
    emit modeChanged(mode);
  }
}

void TableView::hideColumns() {
  for (auto column : columns)
    setColumnHidden(column->getIndex(), !column->getVisible());
}

void TableView::showEvent(QShowEvent *event) {
  Q_UNUSED(event)
  hideColumns();
}

void TableView::resizeEvent(QResizeEvent *event) {
  // setViewportMargins(QMargins(100, 100, 0, 0));
  QTableView::resizeEvent(event);
}
