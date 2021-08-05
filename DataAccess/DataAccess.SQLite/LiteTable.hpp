#pragma once
#include "LiteAdapter.hpp"
#include "QueryBuilder.hpp"

namespace DataAccess
{
/** @class SQLite table.
 *   @brief SQLite table class.
 *   @details You can see some examples below for how to use it.
 *   @code
 *    LiteTable("tableName1").Select()->columns({"column1", "column2",
 * ...})->where("condition")->...->execute()/OR/getQuery(); LiteTable("tableName2").Insert()->field(1,
 * "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
 *    LiteTable("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2",
 * "strData"})->where("condition")->...->execute()/OR/getQuery();
 *    LiteTable("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();
 *   @endcodegit
 */
class LiteTable : public QueryBuilder<QSqlQuery>
{
public:
    LiteTable(const std::string& tableName)
        : QueryBuilder(DatabaseType::DB_LITE, tableName) 
    {
        _adapter = LiteAdapter::getInstance<LiteAdapter>();
    }

    LiteTable(const std::string& tableName, const std::string_view& options)
        : QueryBuilder(DatabaseType::DB_LITE, tableName, options) 
    {
        _adapter = LiteAdapter::getInstance<LiteAdapter>(options);
    }
};
}