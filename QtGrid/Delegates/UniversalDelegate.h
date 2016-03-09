#ifndef UNIVERSALDELEGATE_H
#define UNIVERSALDELEGATE_H

#include <QItemDelegate>

#include "GridColumn.h"

class UniversalDelegate : public QItemDelegate {
  Q_OBJECT
public:
  UniversalDelegate(QObject *parent = 0);

  QWidget *
  createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

  void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

  void updateEditorGeometry(QWidget *editor,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:
  void initDelegate(QObject *const, GridColumn *const, QWidget *) const;
  void endEdit(const QModelIndex &index, GridColumn *const column, const QVariant value) const;
};

#endif // UNIVERSALDELEGATE_H
