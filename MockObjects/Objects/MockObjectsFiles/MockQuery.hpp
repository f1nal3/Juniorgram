#pragma once

#include "MockDatabase.hpp"
#include "MockQueryBuilder.hpp"

namespace MockQuery
{
using DataAccess::IAdapter;
using MockObject::MockRepositoryInstance;
using Database = MockDatabase::MockDatabase;
using QueryBuilder = MockQueryBuilder::MockQueryBuilder<MockRepositoryInstance>;

class MockQuery : public QueryBuilder
{
public: 
    explicit MockQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = Database::getInstance<Database>())
        : MockQueryBuilder("MockDatabase", tableName, adapter) {}
};
}  // namespace MockQuery
