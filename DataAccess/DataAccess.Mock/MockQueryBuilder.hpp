#pragma once 

#include <iostream>
#include <string>

#include <DataAccess/IAdapter.hpp>
#include "MockRepositoryInstance.hpp"

namespace MockObject
{
using DataAccess::IAdapter;

/**
* @brief MockQueryBuilder class.
* @details This class is designed to manage requests coming from repositories. /
*          The SelectAndPushData method checks for the correct repository data table and /
*          adds it for storage (database table emulation), /
*          as well as the data can be taken from the table.
*/
template <typename QueryType = MockRepositoryStorage>
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
        if (!_adapter->query(repoName).has_value())
        {
            return _adapter.get();
        }
        return _adapter->query(repoName)->emplace<std::string>(repoInfo);
    }

    [[nodiscard]] std::shared_ptr<IAdapter>& getAdapter() noexcept { return _adapter; }

private:
    std::shared_ptr<IAdapter> _adapter;
    std::string               _tableName;
    std::string               _databaseType;
};
}  /// namespace MockObject
