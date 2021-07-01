#pragma once
#include <QtSql/QSqlQuery>

#include "QueryBuilder.hpp"
#include "SQLCipherAdapter.hpp"

namespace DataAccess
{
/** @class SQLite table.
 *   @brief SQLite table class.
 *   @details You can see some examples below for how to use it.
 *   @code
 *    LiteTable("tableName1").Select()->columns({"column1", "column2",
 * ...})->where("condition")->...->execute()/OR/getQuery();
 * LiteTable("tableName2").Insert()->field(1, "a")->field(...)->...->returning({"column1",
 * "column2", ...})->execute()/OR/getQuery();
 *    LiteTable("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2",
 * "strData"})->where("condition")->...->execute()/OR/getQuery();
 *    LiteTable("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();
 *   @endcodegit
 */
class SQLCipherTable : public QueryBuilder<QSqlQuery>
{
public:
    SQLCipherTable(const std::string& tableName)
        : QueryBuilder(Utility::DatabaseType::DB_SQLCIPHER, tableName)
    {
        _adapter = SQLCipherAdapter::getInstance<SQLCipherAdapter>();
    }

    SQLCipherTable(const std::string& tableName, const std::string_view& options)
        : QueryBuilder(Utility::DatabaseType::DB_SQLCIPHER, tableName)
    {
        _adapter = SQLCipherAdapter::getInstance<SQLCipherAdapter>(options);
    }
};
}  // namespace DataAccess
