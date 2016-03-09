#include "UniversalTableModel.h"

#include <NullableBase.h>
#include <QDebug>
#include <QMetaProperty>

UniversalTableModel::UniversalTableModel(QObject *parent) : QAbstractTableModel(parent), datasource(nullptr) {
  findMapMethod();
}

int UniversalTableModel::rowCount(const QModelIndex &index) const {
  Q_UNUSED(index);

  if (!datasource)
    return 0;

  return datasource->size();
}

int UniversalTableModel::columnCount(const QModelIndex &index) const {
  Q_UNUSED(index);

  return columns.count();
}

QVariant UniversalTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (index.row() >= datasource->size() || index.column() >= columns.count())
    return QVariant();

  QVariant result;
  QVariant currentValue =
      datasource->value(index.row())->property(columns.at(index.column())->getFieldName().toStdString().data());
  if (currentValue.canConvert(QMetaType::type("NullableBase"))) {
    if (currentValue.isValid()) {
      NullableBase nullableBaseValue = currentValue.value<NullableBase>();
      if (&nullableBaseValue)
        currentValue = nullableBaseValue.getVariant();
      else
        currentValue = QVariant();
    }
  }
  GridColumn *column = columns[index.column()];
  switch (role) {
  case Qt::DisplayRole: {
    if (!column->getCheckable()) {
      if (!column->getProperty().isEmpty()) {
        QObject *o = qvariant_cast<QObject *>(currentValue);
        if (o != nullptr)
          result = o->property(column->getProperty().toStdString().c_str());
      } else {
        result = currentValue;
      }
    }
  } break;
  case Qt::EditRole: {
    result = currentValue;
  } break;
  case Qt::CheckStateRole: {
    if (column->getCheckable())
      result = currentValue.toBool() ? Qt::Checked : Qt::Unchecked;
  } break;
  default:
    break;
  }

  return result;
}

Qt::ItemFlags UniversalTableModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  Qt::ItemFlags flags = QAbstractItemModel::flags(index);

  if (columns[index.column()]->getCheckable())
    flags |= Qt::ItemIsUserCheckable;

  if (columns[index.column()]->getEditable())
    flags |= Qt::ItemIsEditable;

  return flags;
}

bool UniversalTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (index.isValid() && role == Qt::EditRole) {
    datasource->value(index.row())->setProperty(columns.at(index.column())->getFieldName().toStdString().data(), value);
    emit dataChanged(index, index);
  }

  return false;
}

QVariant UniversalTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    if (section < columns.count()) {
      GridColumn *column = columns.at(section);
      if (!column->getHeader().isEmpty() && !column->getHeader().isNull())
        return column->getHeader();
    }
    return QVariant();
  } else {
    return section + 1;
  }
}

ObservableList *UniversalTableModel::getDatasource() const {
  return datasource;
}

void UniversalTableModel::setDatasource(ObservableList *value) {
  if (datasource != value) {
    beginResetModel();
    datasource = value;
    endResetModel();

    emit datasourceChanged();

    subscribeToAllObject();

    connect(datasource, SIGNAL(countChanged()), SLOT(reloadDatasource()), Qt::UniqueConnection);
    connect(datasource, SIGNAL(inserted(int, int)), SLOT(onDatasourceInsert(int, int)), Qt::UniqueConnection);
    connect(datasource, SIGNAL(removed(int, int)), SLOT(onDatasourceRemove(int, int)), Qt::UniqueConnection);
  }
}

GridColumn *UniversalTableModel::getColumn(int index) const {
  return columns.at(index);
}

QObject *UniversalTableModel::getObjectByIndex(const QModelIndex &index) const {
  return datasource->at(index.row());
}

void UniversalTableModel::subscribeToAllObject() {
  for (QObject *obj : *datasource) {
    for (auto col : columns) {
      int propertyIndex = obj->metaObject()->indexOfProperty(col->getFieldName().toStdString().data());
      if (propertyIndex > -1) {
        if (obj->metaObject()->property(propertyIndex).hasNotifySignal()) {
          connect(obj, obj->metaObject()->property(propertyIndex).notifySignal(), &mapper, mapperMapMethod);
          mapper.setMapping(obj, obj);
          connect(&mapper,
                  static_cast<void (QSignalMapper::*)(QObject *)>(&QSignalMapper::mapped),
                  [=]() {
                    int rowIndex = datasource->indexOf(obj);
                    int columnIndex = columns.indexOf(col);
                    QModelIndex modelIndex = index(rowIndex, columnIndex);
                    emit dataChanged(modelIndex, modelIndex);
                  });
        }
      }
    }
  }
}

void UniversalTableModel::findMapMethod() {
  for (int m = 0; m < mapper.metaObject()->methodCount(); m++) {
    QMetaMethod method = mapper.metaObject()->method(m);
    if (method.name() == "map" && method.parameterCount() == 0) {
      mapperMapMethod = method;
      return;
    }
  }
}

void UniversalTableModel::setFilters(QMap<QString, QVariant> value) {
  filters = value;
}

void UniversalTableModel::reloadDatasource() {
  beginResetModel();
  endResetModel();

  emit datasourceChanged();

  subscribeToAllObject();
}

void UniversalTableModel::onDatasourceCountChanged() {
}

void UniversalTableModel::onDatasourceInsert(int index, int count) {
  beginInsertRows(QModelIndex(), index, index + count - 1);
  insertRows(index, count);
  endInsertRows();

  emit rowsInserted();
}

void UniversalTableModel::onDatasourceRemove(int index, int count) {
  beginRemoveRows(QModelIndex(), index, index + count - 1);
  removeRows(index, count);
  endRemoveRows();

  emit rowsRemoved();
}

QList<GridColumn *> UniversalTableModel::getColumns() const {
  return columns;
}

void UniversalTableModel::setColumns(const QList<GridColumn *> &value) {
  beginResetModel();

  columns = value;

  for (auto column : value)
    if (column->getIsId())
      idColumn = column;

  endResetModel();
}

Mode UniversalTableModel::getMode() const {
  return mode;
}

void UniversalTableModel::setMode(const Mode &value) {
  mode = value;
}
