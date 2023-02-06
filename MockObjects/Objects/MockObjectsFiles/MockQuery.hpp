#pragma once

#include "MockDatabase.hpp"

namespace MockQuery
{
using DataAccess::IAdapter;
using Database = MockDatabase::MockDatabase;

class MockQuery
{
public: 
    explicit MockQuery(std::shared_ptr<IAdapter> adapter = Database::getInstance<Database>())
    : _mockAdapter(adapter){}

    std::shared_ptr<IAdapter>& getQuery() { return _mockAdapter; }
private:
    std::shared_ptr<IAdapter> _mockAdapter;
};
}  // namespace MockQuery
