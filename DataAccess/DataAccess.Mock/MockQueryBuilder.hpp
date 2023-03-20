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
* @details This class is designed to manage requests coming from repositories. 
*          The SelectRepoAndQueryPush method checks for the correct repository data table and /
*          adds it for storage (database table emulation), /
*          as well as the data can be taken from the table.
*/
template <typename QueryType = MockRepositoryStorage>
class MockQueryBuilder
{
public:
    MockQueryBuilder() = delete;
    virtual ~MockQueryBuilder() = default;

    MockQueryBuilder(std::shared_ptr<IAdapter> adapter, const std::string& type, const std::string& tableName)
        : _adapter{adapter},  _tableName{tableName}, _databaseType{type} {}

    MockQueryBuilder(const MockQueryBuilder& other)  = delete;
    MockQueryBuilder(MockQueryBuilder&& other)       = delete;

    MockQueryBuilder& operator=(const MockQueryBuilder& other) noexcept = delete;
    MockQueryBuilder& operator=(MockQueryBuilder&& other) noexcept      = delete;

    std::any SelectRepoAndQueryPush(const std::string_view repoName, 
        std::any query, const std::any& repoInfo) const
    {
        if (repoName == "channels")
        {
            _storage.checkChannelTable(std::any_cast<TableChannelRepository>(query), repoInfo);
        }
        if (repoName == "replies")
        {
            _storage.checkRepliesTable(std::any_cast<TableRepliesRepository>(query), repoInfo);
        }
        if (repoName == "msgs")
        {
            _storage.checkMessagesTable(std::any_cast<TableMessagesRepository>(query), repoInfo);
        }
        if (repoName == "user_registration")
        {
            _storage.checkRegistrationTable(std::any_cast<TableRegistrationRepository>(query), repoInfo);
        }
        if (repoName == "user_login")
        {
            _storage.checkLoginTable(std::any_cast<TableLoginRepository>(query), repoInfo);
        }

        return _storage;
    }

    [[nodiscard]] std::shared_ptr<IAdapter>& getAdapter() noexcept { return _adapter; }

    [[nodiscard]] MockRepositoryStorage& getStorage() noexcept { return _storage; }

private:
    inline static MockRepositoryStorage _storage;
    
    std::shared_ptr<IAdapter> _adapter;
    std::string               _tableName;
    std::string               _databaseType;
};
}  /// namespace MockObject
