#pragma once 

#include <DataAccess.Postgre/PostgreTable.hpp>
#include <DataAccess/IAdapter.hpp>
#include <DataAccess.Postgre/UsersAmountFinder.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include <Network/Connection.hpp>
#include <Network/Primitives.hpp>
#include <Cryptography.hpp>
#include <FileLogger.hpp>

namespace MockRepository
{
using IAdapter                 = DataAccess::IAdapter;
using PostgreTable             = DataAccess::PostgreTable;
using IChannelRepository       = DataAccess::IChannelsRepository;
using IDirectMessageRepository = DataAccess::IDirectMessageRepository;
using ILoginRepository         = DataAccess::ILoginRepository;
using IMessagesRepository      = DataAccess::IMessagesRepository;
using IRegisterRepository      = DataAccess::IRegisterRepository;
using IRepliesRepository       = DataAccess::IRepliesRepository;

struct AbstractPostgreRepository
{
protected:
    std::unique_ptr<PostgreTable> _pTable;
};

struct testChannelsRepository : AbstractPostgreRepository, IChannelRepository
{
    explicit testChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter);
    }

    Utility::ChannelDeleteCode        deleteChannel(Network::ChannelDeleteInfo& channel) const;
    Utility::ChannelCreateCodes       createChannel(Network::ChannelInfo& channel) const;
    Utility::ChannelLeaveCodes        leaveChannel(Network::ChannelLeaveInfo& channel) const;
    Utility::ChannelSubscribingCodes  subscribeToChannel(Network::ChannelSubscriptionInfo& channel) const;
    std::vector<uint64_t>             getChannelSubscriptionList(uint64_t userID) const;
    std::vector<Network::ChannelInfo> getAllChannelsList() const;

    ~testChannelsRepository() = default;
};

struct testDirectMessageRepository : AbstractPostgreRepository, IDirectMessageRepository
{
    explicit testDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("channels", adapter);
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t receiverId) const;

    ~testDirectMessageRepository() = default;
};

struct testLoginRepository : AbstractPostgreRepository, ILoginRepository
{
    explicit testLoginRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter); 
    }

    std::uint64_t loginUser(std::string& login, std::string& pwdHash) const;

    ~testLoginRepository() = default;
};

struct testMessagesRepository : AbstractPostgreRepository, IMessagesRepository
{
    explicit testMessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter);
    }

    Utility::DeletingMessageCodes     deleteMessage(Network::MessageInfo& mi) const;
    Utility::ReactionMessageCodes     updateMessageReactions(Network::MessageInfo& mi) const;
    Utility::EditingMessageCodes      editMessage(Network::MessageInfo& mi) const;
    Utility::StoringMessageCodes      storeMessage(Network::MessageInfo& msi) const;
    std::vector<Network::MessageInfo> getMessageHistoryForUser(std::uint64_t channelID) const;

    ~testMessagesRepository() = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Network::MessageInfo& msi) const;
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID) const; 
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID) const;
};

struct testRegisterRepository : AbstractPostgreRepository, IRegisterRepository
{
    explicit testRegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter);
    }

    Utility::RegistrationCodes registerUser(Network::RegistrationInfo& ri) const;

    ~testRegisterRepository() = default;
};

struct testRepliesRepository : AbstractPostgreRepository, IRepliesRepository
{
    explicit testRepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("msgs", adapter); 
    }

    std::vector<Network::ReplyInfo> getReplyHistoryForUser(const std::uint64_t channelID) const;
    Utility::StoringReplyCodes      storeReply(const Network::ReplyInfo& rsi) const;

    ~testRepliesRepository() = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID) const;
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Network::ReplyInfo& rsi) const;
};
}  // namespace MockRepository
