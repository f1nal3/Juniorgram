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
*          The SelectRepoAndQueryPush method checks for the correct repository data table and /
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

    std::any SelectRepoAndQueryPush(const std::string_view& repoName, 
        const std::string_view& query, const std::any& repoInfo)
    {
        if (repoName == "channels")
        {
            if (query == "userID")
            {
                _storage.getChannelsRepoData()._creatorID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getChannelsRepoData();
            }
            if (query == "channelName")
            {
                _storage.getChannelsRepoData()._channelName = std::any_cast<std::string>(repoInfo);
                return _storage.getChannelsRepoData();
            }
            if (query == "channelID")
            {
                _storage.getChannelsRepoData()._channelID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getChannelsRepoData();
            }
            return _storage.getChannelsRepoData();
        }
        if (repoName == "replies")
        {
            if (query == "channelID")
            {
                _storage.getRepliesRepoData()._channelID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getRepliesRepoData();
            }
            if (query == "message")
            {
                _storage.getRepliesRepoData()._message = std::any_cast<std::string>(repoInfo);
                return _storage.getRepliesRepoData();
            }
            if (query == "messageID")
            {
                _storage.getRepliesRepoData()._msgID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getRepliesRepoData();
            }
            if (query == "userLogin")
            {
                _storage.getRepliesRepoData()._userLogin = std::any_cast<std::string>(repoInfo);
                return _storage.getRepliesRepoData();
            }
            return _storage.getRepliesRepoData();
        }
        if (repoName == "msgs")
        {
            if (query == "channelID")
            {
                _storage.getMessageRepoData()._channelID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getMessageRepoData();
            }
            if (query == "channelID")
            {
                _storage.getMessageRepoData()._message = std::any_cast<std::string>(repoInfo);
                return _storage.getMessageRepoData();
            }
            if (query == "messageID")
            {
                _storage.getMessageRepoData()._msgID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getMessageRepoData();
            }
            if (query == "reactions")
            {
                _storage.getMessageRepoData()._reactions 
                    = std::any_cast<std::map<uint32_t, uint32_t>>(repoInfo);
                return _storage.getMessageRepoData();
            }
            if (query == "recipientID")
            {
                _storage.getMessageRepoData()._recipientID = std::any_cast<uint64_t>(repoInfo);
                return _storage.getMessageRepoData();
            }
            if (query == "userLogin")
            {
                _storage.getMessageRepoData()._userLogin = std::any_cast<std::string>(repoInfo);
                return _storage.getMessageRepoData();
            }
            return _storage.getMessageRepoData();
        }
        if (repoName == "user_registration")
        {
            if (query == "login")
            {
                _storage.getRegisterRepoData()._login = std::any_cast<std::string>(repoInfo);
                return _storage.getRegisterRepoData();
            }
            if (query == "email")
            {
                _storage.getRegisterRepoData()._email = std::any_cast<std::string>(repoInfo);
                return _storage.getRegisterRepoData();
            }
            if (query == "password")
            {
                _storage.getRegisterRepoData()._passwordHash = std::any_cast<std::string>(repoInfo);
                return _storage.getRegisterRepoData();
            }
            return _storage.getRegisterRepoData();
        }
        if (repoName == "user_login")
        {
            if (query == "login")
            {
                _storage.getLoginRepoData()._login = std::any_cast<std::string>(repoInfo);
                return _storage.getLoginRepoData();
            }
            if (query == "passwordHash")
            {
                _storage.getLoginRepoData()._pwdHash = std::any_cast<std::string>(repoInfo);
                return _storage.getLoginRepoData();
            }
            return _storage.getLoginRepoData();
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
