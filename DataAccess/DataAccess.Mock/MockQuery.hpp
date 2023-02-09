#pragma once

#include "MockDatabase.hpp"
#include "MockQueryBuilder.hpp"

namespace MockObject
{
using DataAccess::IAdapter;
using MockObject::MockRepositoryInstance;
using Database = MockObject::MockDatabase;
using QueryBuilder = MockObject::MockQueryBuilder<MockRepositoryInstance>;

class MockQuery : public QueryBuilder
{
public: 
    explicit MockQuery(const std::string& tableName, std::shared_ptr<IAdapter> adapter = Database::getInstance<Database>())
        : MockQueryBuilder("MockDatabase", tableName, adapter) {}
};
}  // namespace MockObject
