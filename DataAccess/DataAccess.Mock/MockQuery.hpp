#pragma once

#include "MockDatabase.hpp"
#include "MockQueryBuilder.hpp"

namespace MockObject
{
using DataAccess::IAdapter;
using MockObject::MockRepositoryInstance;
using Database = MockObject::MockDatabase;
using QueryBuilder = MockObject::MockQueryBuilder<MockRepositoryInstance>;

/**
* @brief MockQuery class.
* @details Designed to initialize the database adapter with \
*          the subsequent possible use of methods to process data in the repository.
*/
class MockQuery : public QueryBuilder
{
public: 
    explicit MockQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = Database::getInstance<Database>())
        : MockQueryBuilder(adapter, "MockDatabase", tableName) {}
};
}  /// namespace MockObject
