#ifndef SQL_H
#define SQL_H

#include <QString>

QString sqlCreate =
    "create table TypeA(id serial primary key, code varchar, name varchar);"
    "create table KindA(id serial primary key, code varchar, name varchar, idTypeA integer constraint TypeA_FK "
    "references TypeA(id));"
    "create table A(id serial primary key, code varchar, idKindA integer constraint KindA_FK references KindA(id));"
    "create table B(id serial primary key, idA integer constraint A_FK references KindA(id), code varchar);";

QString sqlFill = "insert into TypeA(code, name) values('code1', 'value1');"
                  "insert into TypeA(code, name) values('code2', 'value2');"
                  "insert into KindA(code, name, idTypeA) values('code1', 'value1', 1);"
                  "insert into KindA(code, name, idTypeA) values('code2', 'value2', 2);"
                  "insert into A(code, idKindA) values('code1', 1);"
                  "insert into A(code, idKindA) values('code2', 2);"
                  "insert into B(idA, code) values(1, 'code1.1');"
                  "insert into B(idA, code) values(1, 'code1.2');"
                  "insert into B(idA, code) values(2, 'code2.1');"
                  "insert into B(idA, code) values(2, 'code2.2');"
                  "insert into B(idA, code) values(2, 'code2.3');";

QString expectedSelectClause = "select tb_1.code as tb_1_code, tb_1.id as tb_1_id"
                               ", tb_2.code as tb_2_code, tb_2.id as tb_2_id, tb_2.name as tb_2_name"
                               ", tb_3.code as tb_3_code, tb_3.id as tb_3_id, tb_3.name as tb_3_name";

QString expectedFromClause = "from A tb_1 left join KindA tb_2 on tb_2.id = tb_1.idKindA "
                             "left join TypeA tb_3 on tb_3.id = tb_2.idTypeA";

QString expectedWhereClause = " A.id = :id and A.code = :code ";

QString expectedInsertText = "insert into A(code, idKindA) "
                             "values(:code, :idKindA) "
                             "returning id";

QString expectedUpdateText = "update A "
                             "set code = :code, idKindA = :idKindA "
                             "where id = :id";

QString expectedDeleteText = "delete from A where id = :id";
#endif // SQL_H
