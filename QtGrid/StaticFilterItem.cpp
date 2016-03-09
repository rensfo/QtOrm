#include "StaticFilterItem.h"

#include <QMetaProperty>
#include <QDebug>

StaticFilterItem::StaticFilterItem(QWidget *parent) : QWidget(parent), editor(nullptr) {
  mapper = new QSignalMapper(this);
  FindMapMethod();
  layout = new QHBoxLayout();
  namePlace = new QLabel();

  layout->addWidget(namePlace);
  layout->addStretch();
  setLayout(layout);
}

QWidget *StaticFilterItem::getEditor() const {
  return editor;
}

void StaticFilterItem::setEditor(QWidget *value) {
  if (editor != value) {
    if (!editor)
      layout->addWidget(value);
    else
      layout->replaceWidget(editor, value);

    editor = value;
    mapSignal(editor);
    if (column->getDefaultValue().isValid())
      editor->setProperty(editor->metaObject()->userProperty().name(), column->getDefaultValue());
    emit changed(column->getFieldName(), getCurrentValue());
  }
}

void StaticFilterItem::mapSignal(QWidget *widget) {
  connect(widget, widget->metaObject()->userProperty().notifySignal(), mapper, mapperMapMethod);
  mapper->setMapping(widget, widget);
  connect(mapper, SIGNAL(mapped(QWidget *)), this, SLOT(receiver(QWidget *)));
}

QVariant StaticFilterItem::getCurrentValue() {
  return getWidgetValue(editor);
}

void StaticFilterItem::FindMapMethod() {
  for (int m = 0; m < mapper->metaObject()->methodCount(); m++) {
    QMetaMethod method = mapper->metaObject()->method(m);
    if (method.name() == "map" && method.parameterCount() == 0) {
      mapperMapMethod = method;
      return;
    }
  }
}

QVariant StaticFilterItem::getWidgetValue(QWidget *widget) {
  return widget->property(widget->metaObject()->userProperty().name());
}

void StaticFilterItem::receiver(QWidget *widget) {
  QVariant value = getWidgetValue(widget);
  emit changed(column->getFieldName(), value);
}
GridColumn *StaticFilterItem::getColumn() const {
  return column;
}

void StaticFilterItem::setColumn(GridColumn *value) {
  column = value;
  namePlace->setText(column->getHeader());
}
