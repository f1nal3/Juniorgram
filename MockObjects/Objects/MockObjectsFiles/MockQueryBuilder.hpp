#pragma once 

#include <iostream>
#include <string>

#include <DataAccess/IAdapter.hpp>
#include "MockRepositoryInstance.hpp"

namespace MockQueryBuilder
{
using DataAccess::IAdapter;

template <typename QueryType = MockObject::MockRepositoryStorage>
class MockQueryBuilder
{
public:
    MockQueryBuilder(const std::string& type, const std::string& tableName, std::shared_ptr<IAdapter> adapter)
        : _databaseType{type}, _tableName{tableName}, _adapter{adapter}
    {
    }

    virtual ~MockQueryBuilder() = default;

    MockQueryBuilder() = delete;

    MockQueryBuilder(const MockQueryBuilder&) = delete;
    MockQueryBuilder(MockQueryBuilder&&)      = delete;

    MockQueryBuilder& operator=(const MockQueryBuilder&) = delete;
    MockQueryBuilder& operator=(MockQueryBuilder&&)      = delete;

    std::optional<std::any> SelectAndPushData(const std::string_view& repoName, const std::string_view& repoInfo)
    {
        return _adapter->query(repoName)->emplace<std::string>(repoInfo);
    }

    [[nodiscard]] std::shared_ptr<IAdapter>& getAdapter() noexcept { return _adapter; }

private:
    std::shared_ptr<IAdapter> _adapter;
    std::string               _tableName;
    std::string               _databaseType;
};
}  // namespace MockQueryBuilder