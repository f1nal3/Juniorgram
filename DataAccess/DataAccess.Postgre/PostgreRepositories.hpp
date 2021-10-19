#pragma once
#include "DataAccess/IServerRepositories.hpp"
#include "PostgreTable.hpp"
#include "UsersAmountFinder.hpp"

namespace DataAccess
{
struct AbstractPostgreRepository
{
protected:
    std::unique_ptr<PostgreTable> pTable;
};

struct ChannelsRepository final : IChannelsRepository, AbstractPostgreRepository
{
    explicit ChannelsRepository(const std::shared_ptr<IAdapter>& adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::vector<Network::ChannelInfo> getAllChannelsList() final;
    Utility::ChannelLeaveCodes        leaveChannel(const Network::ChannelLeaveInfo& channel) final;
    Utility::ChannelSubscribingCodes  subscribeToChannel(const Network::ChannelSubscriptionInfo& channel) final;
    std::vector<uint64_t>             getChannelSubscriptionList(const uint64_t& userID) final;

    Utility::ChannelDeleteCode  deleteChannel(const Network::ChannelDeleteInfo& channel) final;
    Utility::ChannelCreateCodes createChannel(const Network::ChannelInfo& channel) final;

    ~ChannelsRepository() final = default;
};

struct DirectMessageRepository final : IDirectMessageRepository, AbstractPostgreRepository
{
    explicit DirectMessageRepository(const std::shared_ptr<IAdapter>& adapter)
    {
        pTable = std::make_unique<PostgreTable>("channels", adapter);
    }

    int addDirectChat(uint64_t user_id, const Network::DirectMessageInfo& directMessageInfo) final;
    ~DirectMessageRepository() final = default;
};

struct LoginRepository final : ILoginRepository, AbstractPostgreRepository
{
    explicit LoginRepository(const std::shared_ptr<IAdapter>& adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::uint64_t loginUser(const std::string& login, const std::string& pwdHash) final;

    ~LoginRepository() final = default;
};

struct MessagesRepository final : IMessagesRepository, AbstractPostgreRepository
{
    explicit MessagesRepository(const std::shared_ptr<IAdapter>& adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::vector<Network::MessageInfo> getMessageHistory(const std::uint64_t channelID) final;
    Utility::StoringMessageCodes      storeMessage(const Network::MessageInfo& mi) final;
    Utility::DeletingMessageCodes     deleteMessage(const Network::MessageInfo& mi) final;

    ~MessagesRepository() final = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Network::MessageInfo& msi);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};

struct RegisterRepository final : IRegisterRepository, AbstractPostgreRepository
{
    explicit RegisterRepository(const std::shared_ptr<IAdapter>& adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

    Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) final;

    ~RegisterRepository() final = default;
};

struct RepliesRepository final : IRepliesRepository, AbstractPostgreRepository
{
    explicit RepliesRepository(const std::shared_ptr<IAdapter>& adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::vector<Network::ReplyInfo> getReplyHistory(const std::uint64_t channelID) final;
    Utility::StoringReplyCodes      storeReply(const Network::ReplyInfo& rsi) final;

    ~RepliesRepository() final = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID);
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Network::ReplyInfo& rsi);
};
}  // namespace DataAccess
