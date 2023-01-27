#pragma once

#include <DataAccess/AbstractQueryBuilder.hpp>
#include "MockDatabase.hpp"

namespace MockQuery
{
using DataAccess::AbstractQueryBuilder;
using DataAccess::IAdapter;
using Database = MockDatabase::MockDatabase;

/**
 * @class MockQuery
 * @brief MockQuery class.
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
class MockQuery : public AbstractQueryBuilder<pqxx::result>
{
public:
    MockQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = Database::getInstance<Database>())
        : AbstractQueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, adapter){}
    MockQuery(const std::string& tableName, std::string_view options)
        : AbstractQueryBuilder(Utility::DatabaseType::DB_POSTGRE, tableName, Database::getInstance<Database>(options)){}
};
}  // namespace MockQuery
