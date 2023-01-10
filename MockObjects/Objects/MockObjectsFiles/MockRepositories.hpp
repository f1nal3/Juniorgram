#pragma once 

#include <DataAccess/IAdapter.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include "MockQuery.hpp"

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

struct TestAbstractRepository
{
public:
    std::unique_ptr<Query>& getTable() { return _pTable; }
private:
    std::unique_ptr<Query> _pTable;
};

struct testChannelsRepository final : IChannelRepository, TestAbstractRepository
{
    explicit testChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("users", adapter);
    }

    Utility::ChannelDeleteCode        deleteChannel(const Models::ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes       createChannel(const Models::ChannelInfo& channel) override;
    Utility::ChannelLeaveCodes        leaveChannel(const Models::ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes  subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) override;
    std::vector<uint64_t>             getChannelSubscriptionList(const uint64_t userID) override;
    std::vector<Models::ChannelInfo>  getAllChannelsList() override;

    ~testChannelsRepository() override = default;
};

struct testDirectMessageRepository final : IDirectMessageRepository, TestAbstractRepository
{
    explicit testDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("channels", adapter);
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t userID, uint64_t receiverID) override;

    ~testDirectMessageRepository() override = default;
};

struct testLoginRepository final : ILoginRepository, TestAbstractRepository
{
    explicit testLoginRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        getTable() = std::make_unique<Query>("users", adapter); 
    }

    std::uint64_t loginUser(const Models::LoginInfo& loginInfo) override;

    ~testLoginRepository() override = default;
};

struct testMessagesRepository final : IMessagesRepository, TestAbstractRepository
{
    explicit testMessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("users", adapter);
    }

    Utility::DeletingMessageCodes     deleteMessage(const Models::MessageInfo& messageInfo) override;
    Utility::ReactionMessageCodes     updateMessageReactions(const Models::MessageInfo& messageInfo) override;
    Utility::EditingMessageCodes      editMessage(const Models::MessageInfo& messageInfo) override;
    Utility::StoringMessageCodes      storeMessage(const Models::MessageInfo& messageInfo) override;
    std::vector<Models::MessageInfo>  getMessageHistory(const std::uint64_t channelID) override;

    ~testMessagesRepository() override = default;
};

struct testRegisterRepository final : IRegisterRepository, TestAbstractRepository
{
    explicit testRegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        getTable() = std::make_unique<Query>("users", adapter);
    }

    Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) override;

    ~testRegisterRepository() override = default;
};

struct testRepliesRepository final : IRepliesRepository, TestAbstractRepository
{
    explicit testRepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        getTable() = std::make_unique<Query>("msgs", adapter); 
    }

    std::vector<Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override;
    Utility::StoringReplyCodes     storeReply(const Models::ReplyInfo& replyInfo) override;

    ~testRepliesRepository() override = default;
};
}  // namespace TestRepositories
