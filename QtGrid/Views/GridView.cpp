#include "HeaderTableDelegate.h"
#include "GridView.h"
#include "ui_GridView.h"

#include <QDebug>

GridView::GridView(QWidget *parent) : QWidget(parent), ui(new Ui::GridView), searchBarShow(false), staticFilters(nullptr), searchBarFilters(nullptr) {
  ui->setupUi(this);

  ui->staticFilters->hide();
  toolbar = new QToolBar();
  tableModel = new UniversalTableModel(this);
  tableModelHeader = new HeaderTableModel(this);
  ui->tableData->setModel(tableModel);
  ui->tableHeader->setModel(tableModelHeader);

  QBoxLayout *toolboxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
  toolboxLayout->addWidget(toolbar);
  ui->toolbar->setLayout(toolboxLayout);

  connect(ui->staticFilters, SIGNAL(filtersChanged(Group *)), this, SLOT(onChangedStaticFilters(Group *)));
  // connect(tableModel, SIGNAL(datasourceChanged()), ui->tableData, SLOT(resizeColumnsToContents()));
  connect(ui->tableData,
          SIGNAL(initDelegate(QObject * const, GridColumn *const, QWidget *)),
          this,
          SIGNAL(initDelegate(QObject * const, GridColumn *const, QWidget *)));
  connect(ui->tableData,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)),
          this,
          SIGNAL(endEdit(QModelIndex, GridColumn * const, QVariant)));
  connect(ui->tableHeader->horizontalHeader(),
          SIGNAL(sectionResized(int, int, int)),
          this,
          SLOT(onHeaderTableSectionResized(int, int, int)));
  connect(ui->tableData->horizontalHeader(),
          SIGNAL(sectionResized(int, int, int)),
          this,
          SLOT(onDataTableSectionResized(int, int, int)));

  connect(tableModel, SIGNAL(rowsInserted()), this, SLOT(resizeVerticalHeader()));
  connect(tableModel, SIGNAL(rowsRemoved()), this, SLOT(resizeVerticalHeader()));
  connect(tableModel, SIGNAL(datasourceChanged()), this, SLOT(resizeVerticalHeader()));
  connect(tableModelHeader, SIGNAL(filtersChanged(Group * const)), this, SIGNAL(filtersChanged(Group * const)));
}

GridView::~GridView() {
  delete ui;
}

QList<GridColumn *> GridView::getColumns() const {
  return columns;
}

void GridView::setColumns(const QList<GridColumn *> &value) {
  columns = value;
  tableModel->setColumns(columns);
  tableModelHeader->setColumns(columns);
  ui->staticFilters->setFilters(columns);
  ui->staticFilters->setVisible(true);
  tableModel->setFilters(ui->staticFilters->getFilters());

  ui->tableHeader->setColumns(columns);
  ui->tableData->setColumns(columns);
}

Mode GridView::getMode() const {
  return mode;
}

void GridView::setMode(const Mode &value) {
  if (mode != value) {
    tableModel->setMode(mode);
    mode = value;
    recreateToolBox();
    emit modeChanged(mode);
  }
}

void GridView::setDatasource(ObservableList *value) {
  tableModel->setDatasource(value);
  // tableModelHeader->setDatasource(value);
}

ObservableList *GridView::getDatasource() const {
  return tableModel->getDatasource();
}

void GridView::reloadDatasource() {
  ui->tableData->resizeColumnsToContents();
}

void GridView::deleteCurrentRow() {
  emit removeSelectedRow(ui->tableData->currentIndex().row());
}

void GridView::onHeaderTableSectionResized(const int &index, const int &oldSize, const int &newSize) {
  Q_UNUSED(oldSize)

  int currentSize = ui->tableData->horizontalHeader()->sectionSize(index);
  if (currentSize != newSize)
    ui->tableData->horizontalHeader()->resizeSection(index, newSize);
}

void GridView::onDataTableSectionResized(const int &index, const int &oldSize, const int &newSize) {
  Q_UNUSED(oldSize)

  int currentSize = ui->tableHeader->horizontalHeader()->sectionSize(index);
  if (currentSize != newSize)
    ui->tableHeader->horizontalHeader()->resizeSection(index, newSize);
}

void GridView::resizeVerticalHeader() {
  int currentSizeHeader = ui->tableHeader->verticalHeader()->size().width();
  int currentSizeData = ui->tableData->verticalHeader()->sizeHint().width();
  if (currentSizeHeader != currentSizeData) {
    ui->tableHeader->verticalHeader()->setFixedWidth(currentSizeData);
    QMetaObject::invokeMethod(ui->tableHeader, "updateGeometries");
  }
}

void GridView::onChangedStaticFilters(Group *filters) {
  staticFilters = filters;
  filtersChanged(mergeFilters());
}
void GridView::onChangedSearchBarFilters(Group *filters) {
  searchBarFilters = filters;
  filtersChanged(mergeFilters());
}

void GridView::recreateToolBox() {
  toolbar->clear();
  createToolbox();
}

void GridView::createToolbox() {
  QIcon refreshIcon = style()->standardIcon(QStyle::SP_BrowserReload);
  QAction *refresh = new QAction(refreshIcon, "Обновить данные", this);
  connect(refresh, SIGNAL(triggered(bool)), this, SIGNAL(refresh()));
  toolbar->addAction(refresh);
  toolbar->addSeparator();
  if (mode == Mode::ReadWrite) {
    QIcon insertIcon = style()->standardIcon(QStyle::SP_FileIcon);
    QAction *insert = new QAction(insertIcon, "Добавить", this);
    toolbar->addAction(insert);
    connect(insert, SIGNAL(triggered(bool)), this, SIGNAL(insertRow()));

    QIcon delIcon = style()->standardIcon(QStyle::SP_BrowserStop);
    QAction *del = new QAction(delIcon, "Удалить", this);
    toolbar->addAction(del);
    connect(del, SIGNAL(triggered(bool)), this, SLOT(deleteCurrentRow()));
  }
}

void GridView::resizeHeader() {
  int headerHeight = ui->tableHeader->horizontalHeader()->height();
  int filterRowHeight = searchBarShow ? ui->tableHeader->rowHeight(0) : 0;
  ui->tableHeader->setMaximumHeight(headerHeight + filterRowHeight);
}

Group *GridView::mergeFilters() {
  Group *filters = new Group();
  if (staticFilters)
    filters->addGroup(staticFilters);
  if (searchBarFilters)
    filters->addGroup(searchBarFilters);
  filters->setOperation(GroupOperation::And);
  return filters;
}
bool GridView::getSearchBarShow() const {
  return searchBarShow;
}

void GridView::setSearchBarShow(bool value) {
  searchBarShow = value;
  resizeHeader();
}

void GridView::showEvent(QShowEvent *event) {
  Q_UNUSED(event)

  resizeHeader();
}
