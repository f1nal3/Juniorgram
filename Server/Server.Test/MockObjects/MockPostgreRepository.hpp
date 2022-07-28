#pragma once 

#include <DataAccess.Postgre/PostgreTable.hpp>
#include <DataAccess/IAdapter.hpp>
#include <DataAccess.Postgre/UsersAmountFinder.hpp>
#include <DataAccess/IServerRepositories.hpp>

#include <Network/Connection.hpp>
#include <Models/Primitives.hpp>
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

struct testChannelsRepository final : AbstractPostgreRepository, IChannelRepository
{
    explicit testChannelsRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter);
    }

    Utility::ChannelDeleteCode        deleteChannel(Models::ChannelDeleteInfo& channel) const;
    Utility::ChannelCreateCodes       createChannel(Models::ChannelInfo& channel) const;
    Utility::ChannelLeaveCodes        leaveChannel(Models::ChannelLeaveInfo& channel) const;
    Utility::ChannelSubscribingCodes  subscribeToChannel(Models::ChannelSubscriptionInfo& channel) const;
    std::vector<uint64_t>             getChannelSubscriptionList(uint64_t userID) const;
    std::vector<Models::ChannelInfo>  getAllChannelsList() const;

    ~testChannelsRepository() = default;
};

struct testDirectMessageRepository final : AbstractPostgreRepository, IDirectMessageRepository
{
    explicit testDirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("channels", adapter);
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t receiverId) const;

    ~testDirectMessageRepository() = default;
};

struct testLoginRepository final : AbstractPostgreRepository, ILoginRepository
{
    explicit testLoginRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter); 
    }

    std::uint64_t loginUser(std::string& login, std::string& pwdHash) const;

    ~testLoginRepository() = default;
};

struct testMessagesRepository final : AbstractPostgreRepository, IMessagesRepository
{
    explicit testMessagesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter);
    }

    Utility::DeletingMessageCodes     deleteMessage(Models::MessageInfo& mi) const;
    Utility::ReactionMessageCodes     updateMessageReactions(Models::MessageInfo& mi) const;
    Utility::EditingMessageCodes      editMessage(Models::MessageInfo& mi) const;
    Utility::StoringMessageCodes      storeMessage(Models::MessageInfo& msi) const;
    std::vector<Models::MessageInfo>  getMessageHistoryForUser(std::uint64_t channelID) const;

    ~testMessagesRepository() = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Models::MessageInfo& msi) const;
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID) const; 
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID) const;
};

struct testRegisterRepository final : AbstractPostgreRepository, IRegisterRepository
{
    explicit testRegisterRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("users", adapter);
    }

    Utility::RegistrationCodes registerUser(Models::RegistrationInfo& ri) const;

    ~testRegisterRepository() = default;
};

struct testRepliesRepository final: AbstractPostgreRepository, IRepliesRepository
{
    explicit testRepliesRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("msgs", adapter); 
    }

    std::vector<Models::ReplyInfo> getReplyHistoryForUser(const std::uint64_t channelID) const;
    Utility::StoringReplyCodes     storeReply(const Models::ReplyInfo& rsi) const;

    ~testRepliesRepository() = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID) const;
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Models::ReplyInfo& rsi) const;
};
}  // namespace MockRepository
