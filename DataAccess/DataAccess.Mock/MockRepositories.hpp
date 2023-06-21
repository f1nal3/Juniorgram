#pragma once 

#include <DataAccess/IAdapter.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include "MockQuery.hpp"
#include "Models/Primitives.hpp"
#include <Models/Models.hpp>

#include "FileLogger.hpp"

namespace MockObject
{
using IAdapter                 = DataAccess::IAdapter;
using IChannelRepository       = DataAccess::IChannelsRepository;
using IDirectMessageRepository = DataAccess::IDirectMessageRepository;
using ILoginRepository         = DataAccess::ILoginRepository;
using IMessagesRepository      = DataAccess::IMessagesRepository;
using IRegisterRepository      = DataAccess::IRegisterRepository;
using IRepliesRepository       = DataAccess::IRepliesRepository;

using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelInfo             = Models::ChannelInfo;
using ChannelDeleteInfo       = Models::ChannelDeleteInfo;
using ChannelLeaveInfo        = Models::ChannelLeaveInfo;
using ChannelSubscriptionInfo = Models::ChannelSubscriptionInfo;

/**
* @brief struct MockChannelsRepository.
* @details Inherited from IChannelRepository struct /
*   (for overriding methods related to this repository).
*/
struct MockChannelsRepository final : DataAccess::IChannelsRepository
{
public:
    explicit MockChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _mockQuery = std::make_unique<MockQuery>("channels", adapter);
    }

    ~MockChannelsRepository() override = default;

private:
    Utility::ChannelDeleteCode       deleteChannel(const ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes      createChannel(const ChannelInfo& channel) override;
    Utility::ChannelLeaveCodes       leaveChannel(const ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes subscribeToChannel(const ChannelSubscriptionInfo& channel) override;
    Utility::ChannelCreateCodes      newCreateChannel(const Models::V2::Channel<>& channel) override;
    std::vector<uint64_t>            getChannelSubscriptionList(const uint64_t userID) override;
    std::vector<Models::ChannelInfo> getAllChannelsList() override;

    std::unique_ptr<MockQuery> _mockQuery;
};

/**
* @brief struct MockDirectMessageRepository.
* @details Inherited from IDirectMessageRepository struct /
*    (for overriding methods related to this repository). 
*/
struct MockDirectMessageRepository final : IDirectMessageRepository
{
public:
    explicit MockDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _mockQuery = std::make_unique<MockQuery>("direct_msgs", adapter);
    }

    ~MockDirectMessageRepository() override = default;

private:
    Utility::DirectMessageStatus addDirectChat(uint64_t userID, uint64_t receiverID) override;

    std::unique_ptr<MockQuery> _mockQuery;
};

/**
* @brief struct MockLoginRepository.
* @details Inherited from ILoginRepository struct /
*   (for overriding methods related to this repository).
*/
struct MockLoginRepository final : ILoginRepository
{
public:
    explicit MockLoginRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        _mockQuery = std::make_unique<MockQuery>("user_login", adapter); 
    }

    ~MockLoginRepository() override = default;

private:
    std::uint64_t loginUser(const LoginInfo& loginInfo) override;

    std::unique_ptr<MockQuery> _mockQuery;
};

/**
* @brief struct MockMessagesRepository.
* @details Inherited from IMessagesRepository struct /
*   (for overriding methods related to this repository). 
*/
struct MockMessagesRepository final : IMessagesRepository
{
public:
    explicit MockMessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _mockQuery = std::make_unique<MockQuery>("msgs", adapter);
    }

    ~MockMessagesRepository() override = default;

private:
    Utility::DeletingMessageCodes    deleteMessage(const MessageInfo& messageInfo) override;
    Utility::ReactionMessageCodes    updateMessageReactions(const MessageInfo& messageInfo) override;
    Utility::EditingMessageCodes     editMessage(const MessageInfo& messageInfo) override;
    Utility::StoringMessageCodes     storeMessage(const MessageInfo& messageInfo) override;
    std::vector<Models::MessageInfo> getMessageHistory(const uint64_t channelID) override;

    std::unique_ptr<MockQuery> _mockQuery;
};

/**
* @brief struct MockRegisterRepository.
* @details Inherited from IRegisterRepository struct /
*   (for overriding methods related to this repository). 
*/
struct MockRegisterRepository final : IRegisterRepository
{
public:
    explicit MockRegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _mockQuery = std::make_unique<MockQuery>("user_registration", adapter);
    }

    ~MockRegisterRepository() override = default;

private:
    Utility::RegistrationCodes registerUser(const RegistrationInfo& regInfo) override;

    std::unique_ptr<MockQuery> _mockQuery;
};

/**
* @brief struct MockRepliesRepository.
* @details Inherited from IRepliesRepository struct /
*   (for overriding methods related to this repository).
*/
struct MockRepliesRepository final : IRepliesRepository
{
public:
    explicit MockRepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        _mockQuery = std::make_unique<MockQuery>("replies", adapter); 
    }
    ~MockRepliesRepository() override = default;

private:
    std::vector<Models::ReplyInfo> getReplyHistory(const uint64_t channelID) override;
    Utility::StoringReplyCodes     storeReply(const ReplyInfo& replyInfo) override;

    std::unique_ptr<MockQuery> _mockQuery;
};
}  /// namespace MockObject
