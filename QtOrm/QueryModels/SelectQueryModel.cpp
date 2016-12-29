#include "SelectQueryModel.h"

#include "ConfigurationMap.h"

namespace QtOrm {
namespace Sql {

using QtOrm::Config::ConfigurationMap;

SelectQueryModel::SelectQueryModel(QObject *parent) : QueryModel(parent) {
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
  return "select " + mainTableModel->getColumnsForSelectClause();
}

QString SelectQueryModel::buildFromClause() {
  return "from " + mainTableModel->getTablesForFromClause();
}

QString SelectQueryModel::buildWhereClause() {
  QSharedPointer<GroupConditions> sharedConditions = QSharedPointer<GroupConditions>::create(conditions);
  QString groupConditionText = GroupConditionToString(sharedConditions);
  if (groupConditionText.isEmpty())
    return QString();

  return "where " + groupConditionText;
}

QString SelectQueryModel::GroupConditionToString(const QSharedPointer<GroupConditions> &groupConditions) {
  QString whereClause;
  for (QSharedPointer<Condition> &condition : groupConditions->getConditions()) {
    QString groupOp = groupOperationToString(groupConditions->getOperation());
    QString conditionText = conditionToString(condition);
    if (whereClause.isEmpty()) {
      whereClause = conditionText;
    } else {
      whereClause += QString(" %1 %2").arg(groupOp).arg(conditionText);
    }
  }

  for (QSharedPointer<GroupConditions> &group : groupConditions->getGroups()) {
    QString groupOp = whereClause.isEmpty() ? "" : groupOperationToString(groupConditions->getOperation());
    QString groupWhere = GroupConditionToString(group);
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

QString SelectQueryModel::conditionToString(QSharedPointer<Condition> &condition) {
  QString conditionString;
  if (condition->getColumn().isEmpty()) {
    condition->setColumn(classBase->getProperty(condition->getProperty())->getColumn());
  }
  QString columnName = condition->getColumn();

  QString tableAlias = mainTableModel->getAlias();
  QString placeHolder = columnName;

  short count = calculateCountUsedColumn(columnName);

  if (count) {
    placeHolder = QString("%1_%2").arg(columnName).arg(count);
  }

  conditionString = condition->toSqlString(tableAlias, placeHolder);

  conditionPlaceholder.insert(condition, placeHolder);

  return conditionString;
}

short SelectQueryModel::calculateCountUsedColumn(const QString &value) {
  short count = 0;

  for (QSharedPointer<Condition> &condition : conditionPlaceholder.keys()) {
    QString columnName = condition->getColumn();
    if (columnName == value) {
      count++;
    }
  }

  return count;
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
  return QString("%1 %2 %3").arg(select).arg(from).arg(where);
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
