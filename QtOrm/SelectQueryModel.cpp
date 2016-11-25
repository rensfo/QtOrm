#include "SelectQueryModel.h"

#include "ConfigurationMap.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Config::ConfigurationMap;

SelectQueryModel::SelectQueryModel(QObject *parent) : QueryModel(parent) {
}

void SelectQueryModel::setAliases(QueryTableModel *tableModel) {
  tableModel->setAlias(generateTableAlias());

  for (QueryJoin *join : tableModel->getJoins()) {
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
  return "select " + mainTableModel->getColumnsForSelectClause();
}

QString SelectQueryModel::buildFromClause() {
  return "from " + mainTableModel->getTablesForFromClause();
}

QString SelectQueryModel::buildWhereClause() {
  QString groupConditionText = GroupConditionToString(conditions);
  if(groupConditionText.isEmpty())
    return QString();

  return "where " + groupConditionText;
}

QString SelectQueryModel::GroupConditionToString(const GroupConditions &groupConditions) {
  QString tableName = mainTableModel->getAlias();
  QString whereClause;
  for (Condition *condition : groupConditions.getConditions()) {
    QString groupOp = groupOperationToString(groupConditions.getOperation());
    QString conditionText = conditionToString(condition);
    if(whereClause.isEmpty())
    {
      whereClause = conditionText;
    }
    else
    {
      whereClause += QString(" %1 %2").arg(groupOp).arg(conditionText);
    }
  }

  for (GroupConditions *group : groupConditions.getGroups()) {
    QString groupOp = whereClause.isEmpty() ? "" : groupOperationToString(groupConditions.getOperation());
    QString groupWhere = GroupConditionToString(*group);
    if (!groupWhere.isEmpty())
      whereClause += QString("%1 (%2)").arg(groupOp).arg(groupWhere);
  }

  return whereClause;
}

QString SelectQueryModel::groupOperationToString(GroupOperation groupOperation) const {
  return groupOperation == GroupOperation::And ? "and" : "or";
}

QString SelectQueryModel::getLikeCondition(const QString &fieldName) const {
  QString tableName = mainTableModel->getName();
  return QString("%1.%2 like '%' || :%2 || '%'").arg(tableName).arg(fieldName);
}

QString SelectQueryModel::operationToString(const Condition &filter) const {
  QString operationString;
  Operation operation = filter.getOperation();
  if (operation == Operation::Equal) {
    operationString = "=";
  } else if (operation == Operation::NotEqual) {
    if (filter.getValues().first().isNull() || !filter.getValues().first().isValid())
      operationString = "is not";
    else
      operationString = "!=";
  } else if (operation == Operation::Like) {
    operationString = "like";
  }
  return operationString;
}

QString SelectQueryModel::conditionToString(Condition *condition)
{
  QString conditionString;
  QString columnName = classBase->getProperty(condition->getPropertyName()).getColumn();

  Operation operation = condition->getOperation();

  if(condition->getValues().first().isNull() || !condition->getValues().first().isValid())
  {
    if(operation == Operation::Equal)
    {
      operation = Operation::IsNull;
    }
    else
    {
      operation = Operation::IsNotNull;
    }
  }

  QString tableAlias = mainTableModel->getAlias();
  QString placeHolder = columnName;
  QString fullColumnName = QString("%1.%2").arg(tableAlias).arg(columnName);

  short count = calculateCountUsedColumn(columnName);

  if(count)
  {
    placeHolder = QString("%1_%2").arg(columnName).arg(count);
  }

  switch(operation)
  {
    case Operation::Like:
      conditionString = QString("%1 %2 '%' || :%3 || '%'").arg(fullColumnName).arg(OperationStrings[operation]);
      conditionPlaceholder.insert(condition, placeHolder);
    break;
    case Operation::IsNull:
    case Operation::IsNotNull:
      conditionString = QString("%1 %2").arg(fullColumnName).arg(OperationStrings[operation]);
    break;
    default:
      conditionString = QString("%1 %2 :%3").arg(fullColumnName).arg(OperationStrings[operation]).arg(placeHolder);
      conditionPlaceholder.insert(condition, placeHolder);
  }

  return conditionString;
}

short SelectQueryModel::calculateCountUsedColumn(const QString &value)
{
  short count = 0;

  for(Condition *condition : conditionPlaceholder.keys())
  {
    QString columnName = classBase->getProperty(condition->getPropertyName()).getColumn();
    if(columnName == value)
    {
      count++;
    }
  }

  return count;
}

QMap<Condition *, QString> SelectQueryModel::getConditionPlaceholder() const
{
  return conditionPlaceholder;
}

void SelectQueryModel::clearPlaceHolders()
{
  conditionPlaceholder.clear();
}

QString SelectQueryModel::getWhere() const {
  return where;
}

void SelectQueryModel::buildModel() {
  mainTableModel = buildQueryTableModel(classBase);
  buildSelectAndFromClause();
}

QueryTableModel *SelectQueryModel::buildQueryTableModel(ClassMapBase *classBase) {
  QueryTableModel *queryTableModel = new QueryTableModel();
  queryTableModel->setName(classBase->getTable());

  for (auto property : classBase->getProperties()) {
    queryTableModel->addColumn(property->getColumn());
  }

  for (auto oneToOne : classBase->getOneToOneRelations()) {
    QueryJoin *join = new QueryJoin();
    join->setType(JoinType::Left);
    join->setLeftTableColumnName(oneToOne->getTableColumn());

    QString property = oneToOne->getProperty();
    QString refClass = ClassMapBase::getTypeNameOfProperty(classBase->getMetaObject(), property);
    ClassMapBase *refClassBase = ConfigurationMap::getMappedClass(refClass);

    join->setRigthTableColumnName(refClassBase->getIdProperty().getColumn());
    join->setQueryTableModel(buildQueryTableModel(refClassBase));

    queryTableModel->appendJoin(join);
  }

  return queryTableModel;
}

QString SelectQueryModel::getSqlText() {
  return select + " " + from + " " + where;
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

  sqlText = select + " " + from;
}

QString SelectQueryModel::getSelect() const {
  return select;
}
}
}
