#include "UniversalDelegate.h"
#include "../Models/UniversalTableModel.h"

#include <QMetaProperty>

#include <QComboBox>
#include <QDebug>
#include <QLineEdit>

UniversalDelegate::UniversalDelegate(QObject *parent) : QItemDelegate(parent) {
}

QWidget *
UniversalDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED(option)

  auto model = static_cast<const UniversalTableModel *>(index.model());
  GridColumn *const column = model->getColumn(index.column());
  auto object = model->getObjectByIndex(index);
  QWidget *widget = column->createNewInstance();
  widget->setParent(parent);
  emit initDelegate(object, column, widget);
  return widget;
}

void UniversalDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  const char *userPropertyName = editor->metaObject()->userProperty().name();
  QVariant data = index.model()->data(index, Qt::EditRole);
  auto model = static_cast<const UniversalTableModel *>(index.model());
  GridColumn *const column = model->getColumn(index.column());
  if (!column->getProperty().isEmpty()) {
    QObject *o = qvariant_cast<QObject *>(data);
    if (o != nullptr)
      data = o->property(column->getProperty().toStdString().c_str());
    else
      data = QVariant();
  }

  editor->setProperty(userPropertyName, data);
}

void UniversalDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  auto universalModel = static_cast<const UniversalTableModel *>(model);
  GridColumn *const column = universalModel->getColumn(index.column());
  QComboBox *cmbx = qobject_cast<QComboBox *>(editor);
  if (cmbx) {
    QVariant value = QVariant(cmbx->currentIndex());
    emit endEdit(index, column, value);
  } else {
    const char *userPropertyName = editor->metaObject()->userProperty().name();
    emit endEdit(index, column, editor->property(userPropertyName));
    // model->setData(index, editor->property(userPropertyName), Qt::EditRole);
  }
}

void UniversalDelegate::updateEditorGeometry(QWidget *editor,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const {
  Q_UNUSED(option)
  Q_UNUSED(index)

  editor->setGeometry(option.rect);
}
