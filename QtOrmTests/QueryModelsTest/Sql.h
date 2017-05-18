#ifndef SQL_H
#define SQL_H

#include <QString>

QString expectedSelectClause = "select tb_1.code as tb_1_code, tb_1.id as tb_1_id"
                               ", tb_2.code as tb_2_code, tb_2.id as tb_2_id, tb_2.name as tb_2_name"
                               ", tb_3.code as tb_3_code, tb_3.id as tb_3_id, tb_3.name as tb_3_name";

QString expectedFromClause = "from A tb_1 left join KindA tb_2 on tb_2.id = tb_1.idKindA "
                             "left join TypeA tb_3 on tb_3.id = tb_2.idTypeA";

QString expectedWhereClause = "where tb_1.id = :id and tb_1.code = :code";

QString expectedHardWhereClause = "where (tb_1.code_1 = :code_1 and tb_1.code_1 > :code_1_1) or (tb_1.code_1 = :code_1_2 and (tb_1.code_1 <= :code_1_3 or tb_1.code_1 != :code_1_4))";

QString expectedOneColumnTwoTimesWhereClause = "where tb_1.code = :code or tb_1.code = :code_1";

QString expectedInsertText = "insert into A(code, idKindA) "
                             "values(:code, :idKindA) "
                             "returning id";

QString expectedUpdateText = "update A "
                             "set code = :code, idKindA = :idKindA "
                             "where id = :id";

QString expectedDeleteText = "delete from A where id = :id";

#endif // SQL_H
