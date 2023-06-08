#pragma once

#include <DataAccess/IServerRepositories.hpp>

#include "PostgreQuery.hpp"
#include "UsersAmountFinder.hpp"
#include "PGModelFiller.hpp"

namespace DataAccess
{
    using Models::New::ChannelData;
    using Models::New::UserChannelsData;

struct AbstractPostgreRepository
{
protected:
    std::unique_ptr<PGQueryBuilder> _pTable;
    std::unique_ptr<PGModelFiller> _filler;

public:
    AbstractPostgreRepository()
    {
        _filler = std::make_unique<PGModelFiller>();
    }
};

struct ChannelsRepository final : IChannelsRepository, AbstractPostgreRepository
{
    explicit ChannelsRepository(const std::shared_ptr<IAdapter>& adapter): AbstractPostgreRepository()
    {
        _pTable = std::make_unique<PGQueryBuilder>("users", adapter); 
    }
    
    std::vector<Models::ChannelInfo>       getAllChannelsList() override;
    Utility::ChannelLeaveCodes             leaveChannel(const Models::ChannelLeaveInfo& channel) override;
    Utility::ChannelSubscribingCodes       subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) override;
    std::vector<uint64_t>                  getChannelSubscriptionList(uint64_t userID) override;

    Utility::ChannelDeleteCode  deleteChannel(const Models::ChannelDeleteInfo& channel) override;
    Utility::ChannelCreateCodes createChannel(const Models::ChannelInfo& channel) override;
    Utility::ChannelCreateCodes newCreateChannel(const Models::New::Channel<>& channel) override;

    ~ChannelsRepository() override = default;
};

struct DirectMessageRepository final : IDirectMessageRepository, AbstractPostgreRepository
{
    explicit DirectMessageRepository(const std::shared_ptr<IAdapter>& adapter) : AbstractPostgreRepository()
    {
        _pTable = std::make_unique<PGQueryBuilder>("channels", adapter);
    }

    Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t receiverID) override;

    ~DirectMessageRepository() override = default;
};

struct LoginRepository : ILoginRepository, AbstractPostgreRepository
{
    explicit LoginRepository(const std::shared_ptr<IAdapter>& adapter) : AbstractPostgreRepository()
    {
        _pTable = std::make_unique<PGQueryBuilder>("users", adapter); 
    }

    std::uint64_t loginUser(const Models::LoginInfo& loginInfo) override;

    ~LoginRepository() = default;
};

struct MessagesRepository final : IMessagesRepository, AbstractPostgreRepository
{
    explicit MessagesRepository(const std::shared_ptr<IAdapter>& adapter) : AbstractPostgreRepository()
    {
        _pTable = std::make_unique<PGQueryBuilder>("users", adapter); 
    }

    std::vector<Models::MessageInfo>       getMessageHistory(const std::uint64_t channelID) override;
    Utility::StoringMessageCodes           storeMessage(const Models::MessageInfo& messageInfo) override;
    Utility::DeletingMessageCodes          deleteMessage(const Models::MessageInfo& messageInfo) override;
    Utility::EditingMessageCodes           editMessage(const Models::MessageInfo& messageInfo) override;
    Utility::ReactionMessageCodes          updateMessageReactions(const Models::MessageInfo& messageInfo) override;

    ~MessagesRepository() override = default;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Models::MessageInfo& messageInfo);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};

struct RegisterRepository final : IRegisterRepository, AbstractPostgreRepository
{
    explicit RegisterRepository(const std::shared_ptr<IAdapter>& adapter) : AbstractPostgreRepository()
    {
        _pTable = std::make_unique<PGQueryBuilder>("users", adapter); 
    }

    Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) override;

    ~RegisterRepository() override = default;
};

struct RepliesRepository final : IRepliesRepository, AbstractPostgreRepository
{
    explicit RepliesRepository(const std::shared_ptr<IAdapter>& adapter) : AbstractPostgreRepository()
    {
        _pTable = std::make_unique<PGQueryBuilder>("msgs", adapter);
    }

    std::vector<Models::ReplyInfo>       getReplyHistory(const std::uint64_t channelID) override;
    Utility::StoringReplyCodes           storeReply(const Models::ReplyInfo& replyInfo) override;

    ~RepliesRepository() override = default;

private:
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID);
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Models::ReplyInfo& replyInfo);
};
}  // namespace DataAccess
