#pragma once

#include <DataAccess/QueryBuilder.hpp>
#include <DataAccess/IAdapter.hpp>
#include <MockPostgreAdapter.hpp>

namespace MockRepository
{
using DataAccess::QueryBuilder;
using DataAccess::IAdapter;

/**
 * @class PostgreTable
 * @brief Postgre table class.
 * @details You can see some examples below for how to use it.
 * @code
 *    MockPostgreQuery("tableName1").Select()->columns({"column1", "column2",
 * ...})->where("condition")->...->execute()/OR/getQuery(); MockPostgreQuery("tableName2").Insert()->field(1,
 * "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
 *    MockPostgreQuery("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2",
 * "strData"})->where("condition")->...->execute()/OR/getQuery();
 *    MockPostgreQuery("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();
 * @endcode
 */
class MockPostgreQuery : public QueryBuilder<pqxx::result>
{
public:
    MockPostgreQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = MockPostgreAdapter::getInstance<MockPostgreAdapter>())
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, adapter)
    {
    }
    MockPostgreQuery(const std::string& tableName, std::string_view options)
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, MockPostgreAdapter::getInstance<MockPostgreAdapter>(options))
    {
    }
};
}  // namespace MockRepository
