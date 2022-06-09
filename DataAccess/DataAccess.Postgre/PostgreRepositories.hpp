#pragma once
#include <DataAccess/IServerRepositories.hpp>

#include "PostgreTable.hpp"
#include "UsersAmountFinder.hpp"

namespace DataAccess
{
struct AbstractPostgreRepository
{
protected:
    std::unique_ptr<PostgreTable> _pTable;
};

struct ChannelsRepository final : IChannelsRepository, AbstractPostgreRepository
{
    explicit ChannelsRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("users", adapter); }
    
    std::vector<Network::ChannelInfo> getAllChannelsList() override;
    Utility::ChannelLeaveCodes        leaveChannel(const Network::ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes  subscribeToChannel(const Network::ChannelSubscriptionInfo& channel) override;
    std::vector<uint64_t>             getChannelSubscriptionList(uint64_t userID) override;

    Utility::ChannelDeleteCode  deleteChannel(const Network::ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes createChannel(const Network::ChannelInfo& channel) override;

    ~ChannelsRepository() override = default;
};

struct DirectMessageRepository final : IDirectMessageRepository, AbstractPostgreRepository
{
    explicit DirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        _pTable = std::make_unique<PostgreTable>("channels", adapter);
    }
    Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t receiverId) override;
    ~DirectMessageRepository() override = default;
};

struct LoginRepository : ILoginRepository, AbstractPostgreRepository
{
    explicit LoginRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::uint64_t loginUser(const Network::LoginInfo& loginInfo) override;

    ~LoginRepository() = default;
};

struct MessagesRepository final : IMessagesRepository, AbstractPostgreRepository
{
    explicit MessagesRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::vector<Network::MessageInfo> getMessageHistory(const std::uint64_t channelID) override;
    Utility::StoringMessageCodes      storeMessage(const Network::MessageInfo& mi) override;
    Utility::DeletingMessageCodes     deleteMessage(const Network::MessageInfo& mi) override;
    Utility::EditingMessageCodes      editMessage(const Network::MessageInfo& mi) override;
    Utility::ReactionMessageCodes     updateMessageReactions(const Network::MessageInfo& mi) override;

    ~MessagesRepository() override = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Network::MessageInfo& msi);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};

struct RegisterRepository final : IRegisterRepository, AbstractPostgreRepository
{
    explicit RegisterRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("users", adapter); }

    Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) override;

    ~RegisterRepository() override = default;
};


struct RepliesRepository final : IRepliesRepository, AbstractPostgreRepository
{
    explicit RepliesRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("msgs", adapter); }

    std::vector<Network::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override;
    Utility::StoringReplyCodes      storeReply(const Network::ReplyInfo& rsi) override;

    ~RepliesRepository() override = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID);
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Network::ReplyInfo& rsi);
};
}  /// namespace DataAccess
