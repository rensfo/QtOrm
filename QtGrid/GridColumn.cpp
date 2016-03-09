#include "GridColumn.h"

GridColumn::GridColumn(QObject *parent)
    : QObject(parent), editable(false), checkable(false), isId(false), visible(true), showInStaticFilter(false) {
}

QString GridColumn::getHeader() const {
  return header;
}

void GridColumn::setHeader(const QString &value) {
  header = value;
}

QString GridColumn::getFieldName() const {
  return fieldName;
}

void GridColumn::setFieldName(const QString &value) {
  fieldName = value;
  if (header.isEmpty())
    header = fieldName;
}

bool GridColumn::getCheckable() const {
  return checkable;
}

void GridColumn::setCheckable(bool value) {
  checkable = value;
}
bool GridColumn::getVisible() const {
  return visible;
}

void GridColumn::setVisible(bool value) {
  visible = value;
}
int GridColumn::getIndex() const {
  return index;
}

void GridColumn::setIndex(int value) {
  index = value;
}
bool GridColumn::getIsId() const {
  return isId;
}

void GridColumn::setIsId(bool value) {
  isId = value;
}

bool GridColumn::getShowInStaticFilter() const {
  return showInStaticFilter;
}

void GridColumn::setShowInStaticFilter(bool value) {
  showInStaticFilter = value;
}

QString GridColumn::getWidgetType() const {
  return widgetType;
}

void GridColumn::setWidgetType(const QString &value) {
  widgetType = value;
}
QVariant GridColumn::getDefaultValue() const {
  return defaultValue;
}

void GridColumn::setDefaultValue(const QVariant &value) {
  defaultValue = value;
}
QMetaObject GridColumn::getDelegateMetaObject() const {
  return delegateMetaObject;
}

void GridColumn::setDelegateMetaObject(const QMetaObject &value) {
  delegateMetaObject = value;
}

QString GridColumn::getProperty() const {
  return property;
}

void GridColumn::setProperty(const QString &value) {
  property = value;
}
bool GridColumn::getEditable() const {
  return editable;
}

void GridColumn::setEditable(bool value) {
  editable = value;
}

// QWidget *GridColumn::getDelegate() const
//{
//    return delegate;
//}

// void GridColumn::setDelegate(QWidget *value)
//{
//    delegate = value;
//}
