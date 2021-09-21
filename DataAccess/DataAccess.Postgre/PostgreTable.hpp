#pragma once
#include "PostgreAdapter.hpp"
#include "QueryBuilder.hpp"

namespace DataAccess
{
/** @class Postgre table.
 *   @brief Postgre table class.
 *   @details You can see some examples below for how to use it.
 *   @code
 *    PostgreTable("tableName1").Select()->columns({"column1", "column2",
 * ...})->where("condition")->...->execute()/OR/getQuery(); PostgreTable("tableName2").Insert()->field(1,
 * "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
 *    PostgreTable("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2",
 * "strData"})->where("condition")->...->execute()/OR/getQuery();
 *    PostgreTable("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();
 *   @endcode
 */
class PostgreTable : public QueryBuilder<pqxx::result>
{
public:
    PostgreTable(const std::string& tableName, std::shared_ptr<IAdapter> adapter)
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, adapter) {}

    PostgreTable(const std::string& tableName, const std::string_view& options, std::shared_ptr<IAdapter> adapter)
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, adapter) {}
};
}
