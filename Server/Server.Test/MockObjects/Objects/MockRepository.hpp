#pragma once 

#include <DataAccess/IAdapter.hpp>
#include <DataAccess/IServerRepositories.hpp>
#include "MockQuery.hpp"

namespace MockRepository
{
using IAdapter                 = DataAccess::IAdapter;
using IChannelRepository       = DataAccess::IChannelsRepository;
using IDirectMessageRepository = DataAccess::IDirectMessageRepository;
using ILoginRepository         = DataAccess::ILoginRepository;
using IMessagesRepository      = DataAccess::IMessagesRepository;
using IRegisterRepository      = DataAccess::IRegisterRepository;
using IRepliesRepository       = DataAccess::IRepliesRepository;
using Query                    = MockQuery::MockQuery;

struct MockAbstractRepository
{
protected:
    std::unique_ptr<Query> _pTable;
};

struct testChannelsRepository final : IChannelRepository, MockAbstractRepository
{
    explicit testChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("users", adapter);
    }

    Utility::ChannelDeleteCode        deleteChannel(const Models::ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes       createChannel(const Models::ChannelInfo& channel) override;
    Utility::ChannelLeaveCodes        leaveChannel(const Models::ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes  subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) override;
    std::vector<uint64_t>             getChannelSubscriptionList(const uint64_t userID) override;
    std::vector<Models::ChannelInfo>  getAllChannelsList() override;

    ~testChannelsRepository() = default;
};

struct testDirectMessageRepository final : IDirectMessageRepository, MockAbstractRepository
{
    explicit testDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("channels", adapter);
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t userID, uint64_t receiverID) override;

    ~testDirectMessageRepository() = default;
};

struct testLoginRepository final : ILoginRepository, MockAbstractRepository
{
    explicit testLoginRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        _pTable = std::make_unique<Query>("users", adapter); 
    }

    std::uint64_t loginUser(const Models::LoginInfo& loginInfo) override;

    ~testLoginRepository() = default;
};

struct testMessagesRepository final : IMessagesRepository, MockAbstractRepository
{
    explicit testMessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("users", adapter);
    }

    Utility::DeletingMessageCodes     deleteMessage(const Models::MessageInfo& messageInfo) override;
    Utility::ReactionMessageCodes     updateMessageReactions(const Models::MessageInfo& messageInfo) override;
    Utility::EditingMessageCodes      editMessage(const Models::MessageInfo& messageInfo) override;
    Utility::StoringMessageCodes      storeMessage(const Models::MessageInfo& messageInfo) override;
    std::vector<Models::MessageInfo>  getMessageHistory(const std::uint64_t channelID) override;

    ~testMessagesRepository() = default;

private:
    std::optional<pqxx::result> testInsertMessageIntoMessagesTable(const Models::MessageInfo& messageInfo);
    std::optional<pqxx::result> testInsertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID); 
    std::optional<pqxx::result> testInsertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};

struct testRegisterRepository final : IRegisterRepository, MockAbstractRepository
{
    explicit testRegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("users", adapter);
    }

    Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) override;

    ~testRegisterRepository() = default;
};

struct testRepliesRepository final : IRepliesRepository, MockAbstractRepository
{
    explicit testRepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        _pTable = std::make_unique<Query>("msgs", adapter); 
    }

    std::vector<Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override;
    Utility::StoringReplyCodes     storeReply(const Models::ReplyInfo& replyInfo) override;

    ~testRepliesRepository() = default;

private:
    std::optional<pqxx::result> testInsertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID) ;
    std::optional<pqxx::result> testInsertReplyIntoRepliesTable(const Models::ReplyInfo& replyInfo);
};
}  // namespace MockRepository
