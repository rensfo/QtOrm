#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QToolBar>
#include <QWidget>

#include "ObservableList.h"
#include "HeaderTableModel.h"
#include "UniversalTableModel.h"

namespace Ui {
class GridView;
}

class GridView : public QWidget {
  Q_OBJECT

public:
  explicit GridView(QWidget *parent = 0);
  ~GridView();

  QList<GridColumn *> getColumns() const;
  void setColumns(const QList<GridColumn *> &value);

  Mode getMode() const;
  void setMode(const Mode &value);

  void setDatasource(ObservableList *value);
  ObservableList *getDatasource() const;

  bool getSearchBarShow() const;
  void setSearchBarShow(bool value);

public slots:
  void reloadDatasource();
  void deleteCurrentRow();

private slots:
  void onHeaderTableSectionResized(const int &index, const int &oldSize, const int &newSize);
  void onDataTableSectionResized(const int &index, const int &oldSize, const int &newSize);
  void resizeVerticalHeader();
  void onChangedStaticFilters(Group *filters);
  void onChangedSearchBarFilters(Group *filters);

private:
  void recreateToolBox();
  void createToolbox();
  void resizeHeader();
  Group * converFilters(const QMap<QString, QVariant> &filters);
  Group * mergeFilters();

signals:
  void modeChanged(Mode);
  void initDelegate(QObject *const, GridColumn *const, QWidget *) const;
  void endEdit(const QModelIndex &index, GridColumn *const column, const QVariant value) const;
  void removeSelectedRow(int row);
  void insertRow();
  void filtersChanged(Group *const);
  void refresh();

private:
  Ui::GridView *ui;
  QList<GridColumn *> columns;
  Mode mode;
  UniversalTableModel *tableModel;
  HeaderTableModel *tableModelHeader;
  QToolBar *toolbar;
  bool searchBarShow;
  Group * staticFilters;
  Group * searchBarFilters;

  // QWidget interface
protected:
  void showEvent(QShowEvent *event);
};

#endif // GRIDVIEW_H
