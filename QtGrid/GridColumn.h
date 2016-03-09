#ifndef GRIDCOLUMN_H
#define GRIDCOLUMN_H

#include <QObject>
#include <QVariant>

class FactoryBase {
public:
  virtual QWidget *createNewInstance() const = 0;
};

template <class T> class Factory : public FactoryBase {
public:
  QWidget *createNewInstance() const {
    return new T();
  }
};

class GridColumn : public QObject {
  Q_OBJECT
public:
  explicit GridColumn(QObject *parent = 0);

  QString getHeader() const;
  void setHeader(const QString &value);

  QString getFieldName() const;
  void setFieldName(const QString &value);

  bool getCheckable() const;
  void setCheckable(bool value);

  bool getVisible() const;
  void setVisible(bool value);

  int getIndex() const;
  void setIndex(int value);

  bool getIsId() const;
  void setIsId(bool value);

  bool getShowInStaticFilter() const;
  void setShowInStaticFilter(bool value);

  QString getWidgetType() const;
  void setWidgetType(const QString &value);

  QVariant getDefaultValue() const;
  void setDefaultValue(const QVariant &value);

  template <class T> void setDelegate() {
    f = new Factory<T>();
  }

  QWidget *createNewInstance() {
    return f->createNewInstance();
  }

  QMetaObject getDelegateMetaObject() const;
  void setDelegateMetaObject(const QMetaObject &value);

  QString getProperty() const;
  void setProperty(const QString &value);

  bool getEditable() const;
  void setEditable(bool value);

signals:

public slots:

private:
  QString header;
  QString fieldName;
  bool editable;
  bool checkable;
  bool visible;
  int index;
  bool isId;
  bool showInStaticFilter;
  QString widgetType;
  QVariant defaultValue;
  QWidget *delegate;
  QMetaObject delegateMetaObject;
  FactoryBase *f;
  QString property;
};

#endif // GRIDCOLUMN_H
