#pragma once

#include <DataAccess/QueryBuilder.hpp>
#include "TestDatabase.hpp"

namespace TestQuery
{
using DataAccess::QueryBuilder;
using DataAccess::IAdapter;
using Database = TestDatabase::TestDatabase;

/**
 * @class TestQuery
 * @brief TestQuery class.
 * @details You can see some examples below for how to use it.
 * @code
 *    MockQuery("tableName1").Select()->columns({"column1", "column2",
 * ...})->where("condition")->...->execute()/OR/getQuery(); MockQuery("tableName2").Insert()->field(1,
 * "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
 *    MockQuery("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2",
 * "strData"})->where("condition")->...->execute()/OR/getQuery();
 *    MockQuery("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();
 * @endcode
 */
class TestQuery : public QueryBuilder<pqxx::result>
{
public:
    TestQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = Database::getInstance<Database>())
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, adapter)
    {
    }
    TestQuery(const std::string& tableName, std::string_view options)
        : QueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, Database::getInstance<Database>(options))
    {
    }
};
}  // namespace MockQuery
