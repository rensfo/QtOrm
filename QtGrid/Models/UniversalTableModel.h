#ifndef UNIVERSALTABLEMODEL_H
#define UNIVERSALTABLEMODEL_H

#include "ObservableList.h"
#include <QAbstractTableModel>
#include <QMetaProperty>
#include <QSignalMapper>

#include "GridColumn.h"
//#include "Editors/EditorFactory.h"

enum class Mode { Read, ReadWrite };


class UniversalTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    UniversalTableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &index) const;
    int columnCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Mode getMode() const;
    void setMode(const Mode &value);
    QList<GridColumn *> getColumns() const;
    void setColumns(const QList<GridColumn *> &value);

    ObservableList *getDatasource() const;
    void setDatasource(ObservableList *value);

    GridColumn *getColumn(int index) const;
    QObject * getObjectByIndex(const QModelIndex &index) const;

private:
    void subscribeToAllObject();
    void findMapMethod();

signals:
    void datasourceChanged();
    void rowsInserted();

    void rowsRemoved();
public slots:
    void setFilters(QMap<QString, QVariant> value);
    void reloadDatasource();

private slots:
    void onDatasourceCountChanged();
    void onDatasourceInsert(int index, int count);
    void onDatasourceRemove(int index, int count);

private:
    QString table;
    QList<GridColumn *> columns;
    QMap<QString, QVariant> filters;
    Mode mode;
    GridColumn *idColumn;
    ObservableList *datasource;
    QSignalMapper mapper;
    QMetaMethod mapperMapMethod;
};


#endif // UNIVERSALTABLEMODEL_H
