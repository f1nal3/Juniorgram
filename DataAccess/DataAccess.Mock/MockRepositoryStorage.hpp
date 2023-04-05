#pragma once 

#include <string>
#include <vector>

#include <Models/Primitives.hpp>
#include "MockTables.hpp"

namespace MockObject
{
using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelInfo             = Models::ChannelInfo;

/**
* @brief MockRepositoryStorage class.
* @details Designed to initialize tables (data strings) that /
*          emulate database storage for a specific repository.
*/
struct MockRepositoryStorage
{
    MockRepositoryStorage()  = default;
    ~MockRepositoryStorage() = default;

    ChannelInfo&                     getChannelsRepoData() { return _channelData; }
    ReplyInfo&                       getRepliesRepoData() { return _repliesData; }
    MessageInfo&                     getMessageRepoData() { return _messgData; }
    RegistrationInfo&                getRegisterRepoData() { return _regData; }
    LoginInfo&                       getLoginRepoData() { return _loginData; }

    ChannelInfo& checkChannelTable(TableChannelRepository table, const std::any& repoInfo)
    {
        switch (table)
        {
            case TableChannelRepository::UserID:
                getChannelsRepoData()._creatorID = std::any_cast<uint64_t>(repoInfo);
                return getChannelsRepoData();
                break;

            case TableChannelRepository::ChannelID:
                getChannelsRepoData()._channelID   = std::any_cast<uint64_t>(repoInfo);
                return getChannelsRepoData();
                break;

            case TableChannelRepository::ChanneName:
                getChannelsRepoData()._channelName = std::any_cast<std::string>(repoInfo);
                return getChannelsRepoData();
                break;

            default:
                return getChannelsRepoData();
                break;
        }
    }

    ReplyInfo& checkRepliesTable(TableRepliesRepository table, const std::any& repoInfo)
    {
        switch (table)
        {
            case TableRepliesRepository::ChannelID:
                getRepliesRepoData()._channelID = std::any_cast<uint64_t>(repoInfo);
                return getRepliesRepoData();
                break;
            case TableRepliesRepository::Message:
                getRepliesRepoData()._message = std::any_cast<std::string>(repoInfo);
                return getRepliesRepoData();
                break;
            case TableRepliesRepository::MessageID:
                getRepliesRepoData()._msgID = std::any_cast<uint64_t>(repoInfo);
                return getRepliesRepoData();
                break;
            case TableRepliesRepository::UserLogin:
                getRepliesRepoData()._userLogin = std::any_cast<std::string>(repoInfo);
                return getRepliesRepoData();
                break;
            default:
                return getRepliesRepoData();
                break;
        }
    }

    MessageInfo& checkMessagesTable(TableMessagesRepository table, const std::any& repoInfo)
    {
        switch (table)
        {
            case TableMessagesRepository::UserID:
                getMessageRepoData()._senderID = std::any_cast<uint64_t>(repoInfo);
                return getMessageRepoData();
                break;
            case TableMessagesRepository::ChannelID:
                getMessageRepoData()._channelID = std::any_cast<uint64_t>(repoInfo);
                return getMessageRepoData();
                break;
            case TableMessagesRepository::Message:
                getMessageRepoData()._message = std::any_cast<std::string>(repoInfo);
                return getMessageRepoData();
                break;
            case TableMessagesRepository::MessageID:
                getMessageRepoData()._msgID = std::any_cast<uint64_t>(repoInfo);
                return getMessageRepoData();
                break;
            case TableMessagesRepository::Reactions:
                getMessageRepoData()._reactions = std::any_cast<std::map<uint32_t, uint32_t>>(repoInfo);
                return getMessageRepoData();
                break;
            case TableMessagesRepository::RecipientID:
                getMessageRepoData()._recipientID = std::any_cast<uint64_t>(repoInfo);
                return getMessageRepoData();
                break;
            case TableMessagesRepository::UserLogin:
                getMessageRepoData()._userLogin = std::any_cast<std::string>(repoInfo);
                return getMessageRepoData();
                break;
            default:
                return getMessageRepoData();
                break;
        }
    }

    RegistrationInfo& checkRegistrationTable(TableRegistrationRepository table, const std::any& repoInfo)
    {
        switch (table)
        {
            case TableRegistrationRepository::UserLogin:
                getRegisterRepoData()._login = std::any_cast<std::string>(repoInfo);
                return getRegisterRepoData();
                break;
            case TableRegistrationRepository::Email:
                getRegisterRepoData()._email = std::any_cast<std::string>(repoInfo);
                return getRegisterRepoData();
                break;
            case TableRegistrationRepository::Password:
                getRegisterRepoData()._passwordHash = std::any_cast<std::string>(repoInfo);
                return getRegisterRepoData();
                break;
            default:
                return getRegisterRepoData();
                break;
        }
    }

    LoginInfo& checkLoginTable(TableLoginRepository table, const std::any& repoInfo)
    {
        switch (table)
        {
            case TableLoginRepository::UserLogin:
                getLoginRepoData()._login = std::any_cast<std::string>(repoInfo);
                return getLoginRepoData();
                break;
            case TableLoginRepository::PasswordHash:
                getLoginRepoData()._pwdHash = std::any_cast<std::string>(repoInfo);
                return getLoginRepoData();
                break;
            default:
                return getLoginRepoData();
                break;
        }
    }
private:
    ChannelInfo                     _channelData;
    ReplyInfo                       _repliesData;
    MessageInfo                     _messgData;
    RegistrationInfo                _regData;
    LoginInfo                       _loginData;
};
}  /// namespace MockObject
