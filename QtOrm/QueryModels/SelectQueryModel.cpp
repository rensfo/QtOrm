#include "SelectQueryModel.h"

#include "ConfigurationMap.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Config::ConfigurationMap;

const QMap<Operation, QString> conditionSqlTemplates{{Operation::Between, QString("%1.%2 between :%3_1 and :%3_2 ")},
                                                     {Operation::Equal, QString("%1.%2 = :%3")},
                                                     {Operation::Greater, QString("%1.%2 > :%3")},
                                                     {Operation::GreaterOrEqual, QString("%1.%2 >= :%3")},
                                                     {Operation::In, QString("%1.%2  in (%3)")},
                                                     {Operation::IsNotNull, QString("%1.%2 is not null")},
                                                     {Operation::IsNull, QString("%1.%2 is null")},
                                                     {Operation::Less, QString("%1.%2 < :%3")},
                                                     {Operation::LessOrEqual, QString("%1.%2 <= :%3")},
                                                     {Operation::Like, QString("%1.%2 like '%' || :%3 || '%'")},
                                                     {Operation::NotEqual, QString("%1.%2 != :%3")}};

const QMap<Sort, QString> sortStrings{{Sort::ASC, "ASC"}, {Sort::DESC, "DESC"}};

const QMap<GroupOperation, QString> groupOperationStrings{{GroupOperation::And, "and"}, {GroupOperation::Or, "or"}};

SelectQueryModel::SelectQueryModel() : QueryModel() {
}

SelectQueryModel::~SelectQueryModel() {
}

void SelectQueryModel::setAliases(QSharedPointer<QueryTableModel> tableModel) {
  tableModel->setAlias(generateTableAlias());

  for (QSharedPointer<QueryJoin> join : tableModel->getJoins()) {
    setAliases(join->getQueryTableModel());
  }
}

QString SelectQueryModel::generateTableAlias() {
  return tableAliasTemplate.arg(++tableNumber);
}

void SelectQueryModel::resetTableNumber() {
  tableNumber = 0;
}

QString SelectQueryModel::buildSelectClause() {
  return selectTemplate.arg(mainTableModel->getColumnsForSelectClause());
}

QString SelectQueryModel::buildFromClause() {
  return fromTemplate.arg(mainTableModel->getTablesForFromClause());
}

QString SelectQueryModel::buildWhereClause() {
  if (conditions.isEmpty())
    return QString();

  QSharedPointer<GroupConditions> sharedConditions = QSharedPointer<GroupConditions>::create(conditions);
  QString groupConditionText = groupConditionToString(sharedConditions);

  return whereTemplate.arg(groupConditionText);
}

QString SelectQueryModel::buildOrderByClause() {
  if (!orderColumns.count())
    return QString();

  QStringList orderColumnStrings;
  for (auto orderColumn : orderColumns) {
    orderColumnStrings << orderColumnToString(orderColumn);
  }

  return orderByTemplate.arg(orderColumnStrings.join(", "));
}

QString SelectQueryModel::orderColumnToString(OrderColumn &orderColumn) {
  return orderColumnTemplate.arg(mainTableModel->getAlias())
      .arg(orderColumn.orderProperty)
      .arg(sortStrings[orderColumn.sort]);
}

QString SelectQueryModel::groupConditionToString(const QSharedPointer<GroupConditions> &groupConditions) {
  if (groupConditions->isEmpty())
    return QString();

  QStringList conditionStringList;
  for (QSharedPointer<Condition> &condition : groupConditions->getConditions()) {
    conditionStringList << conditionToString(condition);
  }

  for (QSharedPointer<GroupConditions> &group : groupConditions->getGroups()) {
    if (group->isEmpty())
      continue;

    conditionStringList << QString("(%1)").arg(groupConditionToString(group));
  }

  QString groupOp = groupOperationStrings[groupConditions->getOperation()];
  return conditionStringList.join(QString(" %1 ").arg(groupOp));
}

QString SelectQueryModel::conditionToString(QSharedPointer<Condition> &condition) {
  QString conditionString;
  QString columnName = condition->getProperty();

  QString tableAlias = mainTableModel->getAlias();
  QString placeHolder = columnName;

  short count = calculateCountUsedColumn(columnName);

  if (count) {
    placeHolder = QString("%1_%2").arg(columnName).arg(count);
  }

  conditionString = conditionToStringBase(condition, tableAlias, placeHolder);

  conditionPlaceholder.insert(condition, placeHolder);

  return conditionString;
}

QString SelectQueryModel::conditionToStringBase(QSharedPointer<Condition> &condition, const QString tableAlias,
                                                const QString &placeholder) {
  QString result, sqlTemplate = conditionSqlTemplates[condition->getOperation()], property = condition->getProperty();
  Operation operation = condition->getOperation();
  switch (operation) {
  case Operation::IsNotNull:
  case Operation::IsNull:
    result = sqlTemplate.arg(tableAlias).arg(property);
    break;
  case Operation::Equal:
  case Operation::Greater:
  case Operation::GreaterOrEqual:
  case Operation::Less:
  case Operation::LessOrEqual:
  case Operation::Like:
  case Operation::NotEqual:
    result = sqlTemplate.arg(tableAlias).arg(property).arg(placeholder);
    break;
  case Operation::Between:
    result = sqlTemplate.arg(tableAlias).arg(property).arg(placeholder);
    break;
  case Operation::In: {
    QStringList placeholders;
    for (int i = 1; i <= condition->getValues().count(); ++i) {
      placeholders << QString(":%1_%2").arg(placeholder).arg(i);
    }

    result = sqlTemplate.arg(tableAlias).arg(property).arg(placeholders.join(", "));
    break;
  }
  }

  return result;
}

short SelectQueryModel::calculateCountUsedColumn(const QString &value) {
  short count = 0;

  for (QSharedPointer<Condition> &condition : conditionPlaceholder.keys()) {
    QString columnName = condition->getProperty();
    if (columnName == value) {
      count++;
    }
  }

  return count;
}

QList<OrderColumn> SelectQueryModel::getOrderColumns() const {
  return orderColumns;
}

void SelectQueryModel::setOrderColumns(const QList<OrderColumn> &value) {
  orderColumns = value;
  orderBy = buildOrderByClause();
}

QMap<QSharedPointer<Condition>, QString> SelectQueryModel::getConditionPlaceholder() const {
  return conditionPlaceholder;
}

void SelectQueryModel::clearPlaceHolders() {
  conditionPlaceholder.clear();
}

QString SelectQueryModel::getWhere() const {
  return where;
}

void SelectQueryModel::buildModel() {
  mainTableModel = buildQueryTableModel(classBase);
  buildSelectAndFromClause();
}

QSharedPointer<QueryTableModel> SelectQueryModel::buildQueryTableModel(QSharedPointer<ClassMapBase> classBase) {
  QSharedPointer<QueryTableModel> queryTableModel = QSharedPointer<QueryTableModel>::create();
  queryTableModel->setName(classBase->getTable());

  for (auto property : classBase->getProperties()) {
    queryTableModel->addColumn(property->getColumn());
  }

  for (auto oneToOne : classBase->getOneToOneRelations()) {
    QSharedPointer<QueryJoin> join = QSharedPointer<QueryJoin>::create();
    join->setType(JoinType::Left);
    join->setLeftTableColumnName(oneToOne->getTableColumn());

    QString property = oneToOne->getProperty();
    QString refClass = ClassMapBase::getTypeNameOfProperty(classBase->getMetaObject(), property);
    QSharedPointer<ClassMapBase> refClassBase = ConfigurationMap::getMappedClass(refClass);

    join->setRigthTableColumnName(refClassBase->getIdProperty()->getColumn());
    join->setQueryTableModel(buildQueryTableModel(refClassBase));

    queryTableModel->appendJoin(join);
  }

  return queryTableModel;
}

QString SelectQueryModel::getSqlText() {
  return QString("%1 %2 %3 %4").arg(select).arg(from).arg(where).arg(orderBy);
}

GroupConditions SelectQueryModel::getConditions() const {
  return conditions;
}

void SelectQueryModel::setConditions(const GroupConditions &value) {
  conditions = value;
  where = buildWhereClause();
}

QString SelectQueryModel::getFrom() const {
  return from;
}

void SelectQueryModel::buildSelectAndFromClause() {
  resetTableNumber();
  setAliases(mainTableModel);
  select = buildSelectClause();
  from = buildFromClause();

  sqlText = QString("%1 %2").arg(select).arg(from);
}

QString SelectQueryModel::getSelect() const {
  return select;
}
}
}
