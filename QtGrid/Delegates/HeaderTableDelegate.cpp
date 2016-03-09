#include "HeaderTableDelegate.h"

#include <QLineEdit>

HeaderTableDelegate::HeaderTableDelegate(QObject *parent) : QItemDelegate(parent) {
}

QWidget *
HeaderTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED(option)
  Q_UNUSED(index)

  QWidget *widget = new QLineEdit(parent);
  return widget;
}

void HeaderTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
  Q_UNUSED(editor)
  Q_UNUSED(index)
}

void HeaderTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  QLineEdit *le = qobject_cast<QLineEdit *>(editor);
  model->setData(index, le->text());
}

void HeaderTableDelegate::updateEditorGeometry(QWidget *editor,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const {
  Q_UNUSED(option)
  Q_UNUSED(index)

  editor->setGeometry(option.rect);
}
