#ifndef STATICFILTERITEM_H
#define STATICFILTERITEM_H

#include "GridColumn.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMetaProperty>
#include <QSignalMapper>
#include <QVariant>
#include <QWidget>

class StaticFilterItem : public QWidget {
  Q_OBJECT
public:
  explicit StaticFilterItem(QWidget *parent = 0);

  QString getColumnName() const;
  void setColumnName(const QString &value);

  QWidget *getEditor() const;
  void setEditor(QWidget *value);

  QString getFieldName() const;
  void setFieldName(const QString &value);

  void mapSignal(QWidget *widget);

  QVariant getCurrentValue();

  GridColumn *getColumn() const;
  void setColumn(GridColumn *value);

private:
  void FindMapMethod();
  QVariant getWidgetValue(QWidget *widget);
signals:
  void changed(QString fieldName, QVariant value);
public slots:
  void receiver(QWidget *widget);

private:
  GridColumn *column;
  QWidget *editor;
  QLabel *namePlace;
  QHBoxLayout *layout;
  QSignalMapper *mapper;
  QMetaMethod mapperMapMethod;
};

#endif // STATICFILTERITEM_H
