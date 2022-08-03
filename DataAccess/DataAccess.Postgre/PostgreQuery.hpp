#pragma once
#include <DataAccess/QueryBuilder.hpp>

#include "PostgreAdapter.hpp"

namespace DataAccess
{
/**
 * @class PostgreQuery
 * @brief Postgre query class.
 * @details You can see some examples below for how to use it.
 * @code
 *    PostgreQuery("tableName1").Select()->columns({"column1", "column2",
 * ...})->where("condition")->...->execute()/OR/getQuery(); PostgreTable("tableName2").Insert()->field(1,
 * "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
 *    PostgreQuery("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2",
 * "strData"})->where("condition")->...->execute()/OR/getQuery();
 *    PostgreQuery("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();
 * @endcode
 */
class PostgreQuery : public QueryBuilder<pqxx::result>
{
public:
    PostgreQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = PostgreAdapter::getInstance<PostgreAdapter>())
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, adapter) {}
    PostgreQuery(const std::string& tableName, std::string_view options)
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, PostgreAdapter::getInstance<PostgreAdapter>(options)) {}
};
}  /// namespace DataAccess
