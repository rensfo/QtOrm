#include "StaticFilter.h"
#include "StaticFilterItem.h"

StaticFilter::StaticFilter(QWidget *parent) : QWidget(parent) {
  setLayout(new QVBoxLayout());
}

void StaticFilter::setFilters(const QList<GridColumn *> &columns) {
  filters.clear();
  clearLayout();
  foreach (auto column, columns) {
    if (column->getShowInStaticFilter()) {
      StaticFilterItem *item = new StaticFilterItem();
      connect(item, SIGNAL(changed(QString, QVariant)), this, SLOT(filterItemChanged(QString, QVariant)));
      item->setColumn(column);
      item->setEditor(column->createNewInstance());
      layout()->addWidget(item);
    }
  }
  emitChangedSignal = true;
}

void StaticFilter::clearLayout() {
}

void StaticFilter::filterItemChanged(QString fieldName, QVariant value) {
  filters[fieldName] = value;
  if (emitChangedSignal)
    emit filtersChanged(converFilters());
}

Group *StaticFilter::converFilters()
{
    Group *group = new Group();
    for (auto f = filters.cbegin(); f != filters.cend(); ++f) {
        if (!f.value().toString().isEmpty()) {
            auto filter = new Filter(this);
            filter->setFieldName(f.key());
            filter->setOperation(Operation::Like);
            filter->addValue(f.value());
            group->addFilter(filter);
        }
    }
    return group;
}

QMap<QString, QVariant> StaticFilter::getFilters() const {
  return filters;
}
