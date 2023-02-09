#pragma once 

#include <DataAccess/IAdapter.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include "MockQuery.hpp"
#include "Models/Primitives.hpp"

#include "FileLogger.hpp"

namespace MockRepositories
{
using IAdapter                 = DataAccess::IAdapter;
using IChannelRepository       = DataAccess::IChannelsRepository;
using IDirectMessageRepository = DataAccess::IDirectMessageRepository;
using ILoginRepository         = DataAccess::ILoginRepository;
using IMessagesRepository      = DataAccess::IMessagesRepository;
using IRegisterRepository      = DataAccess::IRegisterRepository;
using IRepliesRepository       = DataAccess::IRepliesRepository;
using Query                    = MockQuery::MockQuery;

/**
* @brief struct MockChannelsRepository.
* @details Inherited from IChannelRepository struct /
*   (for overriding methods related to this repository).
*/
struct MockChannelsRepository final : IChannelRepository
{
public:
    explicit MockChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _mockQuery = std::make_unique<Query>("channels", adapter);
    }

    ~MockChannelsRepository() override = default;

private:
    Utility::ChannelDeleteCode       deleteChannel(const Models::ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes      createChannel(const Models::ChannelInfo& channel) override;
    Utility::ChannelLeaveCodes       leaveChannel(const Models::ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) override;
    std::vector<uint64_t>            getChannelSubscriptionList(const uint64_t userID) override;
    std::vector<Models::ChannelInfo> getAllChannelsList() override;

    std::unique_ptr<Query> _mockQuery;
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
        _mockQuery = std::make_unique<Query>("direct_msgs", adapter);
    }

    ~MockDirectMessageRepository() override = default;

private:
    Utility::DirectMessageStatus addDirectChat(uint64_t userID, uint64_t receiverID) override;

    std::unique_ptr<Query> _mockQuery;
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
        _mockQuery = std::make_unique<Query>("user_login", adapter); 
    }

    ~MockLoginRepository() override = default;

private:
    std::uint64_t loginUser(const Models::LoginInfo& loginInfo) override;

    std::unique_ptr<Query> _mockQuery;
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
        _mockQuery = std::make_unique<Query>("msgs", adapter);
    }

    ~MockMessagesRepository() override = default;

private:
    Utility::DeletingMessageCodes    deleteMessage(const Models::MessageInfo& messageInfo) override;
    Utility::ReactionMessageCodes    updateMessageReactions(const Models::MessageInfo& messageInfo) override;
    Utility::EditingMessageCodes     editMessage(const Models::MessageInfo& messageInfo) override;
    Utility::StoringMessageCodes     storeMessage(const Models::MessageInfo& messageInfo) override;
    std::vector<Models::MessageInfo> getMessageHistory(const std::uint64_t channelID) override;

    std::unique_ptr<Query> _mockQuery;
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
        _mockQuery = std::make_unique<Query>("user_registration", adapter);
    }

    ~MockRegisterRepository() override = default;

private:
    Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) override;

    std::unique_ptr<Query> _mockQuery;
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
        _mockQuery = std::make_unique<Query>("replies", adapter); 
    }
    ~MockRepliesRepository() override = default;

private:
    std::vector<Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override;
    Utility::StoringReplyCodes     storeReply(const Models::ReplyInfo& replyInfo) override;

    std::unique_ptr<Query> _mockQuery;
};
}  // namespace MockRepositories
