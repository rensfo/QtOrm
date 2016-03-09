#ifndef HEADERTABLEDELEGATE_H
#define HEADERTABLEDELEGATE_H

#include <QObject>
#include <QtWidgets/QItemDelegate>

class HeaderTableDelegate : public QItemDelegate {
  Q_OBJECT
public:
  HeaderTableDelegate(QObject *parent = 0);

  QWidget *
  createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

  void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;

  void updateEditorGeometry(QWidget *editor,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // HEADERTABLEDELEGATE_H
