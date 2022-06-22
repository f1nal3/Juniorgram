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
    

    std::vector<Base::Models::ChannelInfo> getAllChannelsList() final;
    Utility::ChannelLeaveCodes             leaveChannel(const Base::Models::ChannelLeaveInfo& channel) final;
    Utility::ChannelSubscribingCodes       subscribeToChannel(const Base::Models::ChannelSubscriptionInfo& channel) final;
    std::vector<uint64_t>                  getChannelSubscriptionList(uint64_t userID) final;

    Utility::ChannelDeleteCode  deleteChannel(const Base::Models::ChannelDeleteInfo& channel) final;
    Utility::ChannelCreateCodes createChannel(const Base::Models::ChannelInfo& channel) final;

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

    std::uint64_t loginUser(const Base::Models::LoginInfo& loginInfo) final;

    ~LoginRepository() = default;
};

struct MessagesRepository final : IMessagesRepository, AbstractPostgreRepository
{
    explicit MessagesRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("users", adapter); }

    std::vector<Base::Models::MessageInfo> getMessageHistory(const std::uint64_t channelID) final;
    Utility::StoringMessageCodes           storeMessage(const Base::Models::MessageInfo& mi) final;
    Utility::DeletingMessageCodes          deleteMessage(const Base::Models::MessageInfo& mi) final;
    Utility::EditingMessageCodes           editMessage(const Base::Models::MessageInfo& mi) final;
    Utility::ReactionMessageCodes          updateMessageReactions(const Base::Models::MessageInfo& mi) final;

    ~MessagesRepository() override = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Base::Models::MessageInfo& msi);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};

struct RegisterRepository final : IRegisterRepository, AbstractPostgreRepository
{
    explicit RegisterRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("users", adapter); }

    Utility::RegistrationCodes registerUser(const Base::Models::RegistrationInfo& ri) final;

    ~RegisterRepository() override = default;
};


struct RepliesRepository final : IRepliesRepository, AbstractPostgreRepository
{
    explicit RepliesRepository(const std::shared_ptr<IAdapter>& adapter) { _pTable = std::make_unique<PostgreTable>("msgs", adapter); }

    std::vector<Base::Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) final;
    Utility::StoringReplyCodes           storeReply(const Base::Models::ReplyInfo& rsi) final;

    ~RepliesRepository() override = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID);
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Base::Models::ReplyInfo& rsi);
};
}  /// namespace DataAccess
