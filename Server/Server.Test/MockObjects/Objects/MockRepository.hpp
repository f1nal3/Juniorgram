#pragma once 

#include <DataAccess/IAdapter.hpp>
#include <DataAccess.Postgre/UsersAmountFinder.hpp>
#include <DataAccess/IServerRepositories.hpp>
#include "MockQuery.hpp"

#include <Network/Connection.hpp>
#include <Models/Primitives.hpp>
#include <Cryptography.hpp>
#include <FileLogger.hpp>

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

struct ChannelsRepository final : IChannelRepository, MockAbstractRepository
{
    explicit ChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("users", adapter);
    }

    Utility::ChannelDeleteCode        deleteChannel(Models::ChannelDeleteInfo& channel) const;
    Utility::ChannelCreateCodes       createChannel(Models::ChannelInfo& channel) const;
    Utility::ChannelLeaveCodes        leaveChannel(Models::ChannelLeaveInfo& channel) const;
    Utility::ChannelSubscribingCodes  subscribeToChannel(Models::ChannelSubscriptionInfo& channel) const;
    std::vector<uint64_t>             getChannelSubscriptionList(uint64_t userID) const;
    std::vector<Models::ChannelInfo>  getAllChannelsList() const;

    ~ChannelsRepository() = default;
};

struct DirectMessageRepository final : IDirectMessageRepository, MockAbstractRepository
{
    explicit DirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("channels", adapter);
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t receiverId) const;

    ~DirectMessageRepository() = default;
};

struct LoginRepository final : ILoginRepository, MockAbstractRepository
{
    explicit LoginRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        _pTable = std::make_unique<Query>("users", adapter); 
    }

    std::uint64_t loginUser(std::string login, std::string pwdHash) const;

    ~LoginRepository() = default;
};

struct MessagesRepository final : IMessagesRepository, MockAbstractRepository
{
    explicit MessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("users", adapter);
    }

    Utility::DeletingMessageCodes     deleteMessage(Models::MessageInfo& mi) const;
    Utility::ReactionMessageCodes     updateMessageReactions(Models::MessageInfo& mi) const;
    Utility::EditingMessageCodes      editMessage(Models::MessageInfo& mi) const;
    Utility::StoringMessageCodes      storeMessage(Models::MessageInfo& msi) const;
    std::vector<Models::MessageInfo>  getMessageHistoryForUser(std::uint64_t channelID) const;

    ~MessagesRepository() = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Models::MessageInfo& msi) const;
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID) const; 
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID) const;
};

struct RegisterRepository final : IRegisterRepository, MockAbstractRepository
{
    explicit RegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<Query>("users", adapter);
    }

    Utility::RegistrationCodes registerUser(Models::RegistrationInfo& ri) const;

    ~RegisterRepository() = default;
};

struct RepliesRepository final : IRepliesRepository, MockAbstractRepository
{
    explicit RepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    { 
        _pTable = std::make_unique<Query>("msgs", adapter); 
    }

    std::vector<Models::ReplyInfo> getReplyHistoryForUser(const std::uint64_t channelID) const;
    Utility::StoringReplyCodes     storeReply(const Models::ReplyInfo& rsi) const;

    ~RepliesRepository() = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID) const;
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Models::ReplyInfo& rsi) const;
};
}  // namespace MockRepository
