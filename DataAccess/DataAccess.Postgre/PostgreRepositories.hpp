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

    struct ChannelsRepository : IChannelsRepository, AbstractPostgreRepository
    {
        ChannelsRepository(std::shared_ptr<IAdapter> adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

        virtual std::vector<Network::ChannelInfo> getAllChannelsList() override final;
        virtual Utility::CreateChannelCodes       createChannel(const Network::ChannelInfo& channel) override final;

        virtual ~ChannelsRepository() = default;
	};

	struct LoginRepository : ILoginRepository, AbstractPostgreRepository
    {
        LoginRepository(std::shared_ptr<IAdapter> adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

        virtual std::uint64_t loginUser(const std::string& login, const std::string& pwdHash) override final;

        virtual ~LoginRepository() = default;
    };

	struct MessagesRepository : IMessagesRepository, AbstractPostgreRepository
    {
        MessagesRepository(std::shared_ptr<IAdapter> adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }
        
        virtual std::vector<Network::MessageInfo> getMessageHistory(const std::uint64_t channelID) override final;
        virtual Utility::StoringMessageCodes      storeMessage(const Network::MessageInfo& mi) override final;
        virtual Utility::DeletingMessageCodes     deleteMessage(const Network::MessageInfo& mi) override final;

        virtual ~MessagesRepository() = default;

    private:

        std::optional<pqxx::result>               insertMessageIntoMessagesTable(const Network::MessageInfo& msi);
        std::optional<pqxx::result>               insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID);
        std::optional<pqxx::result>               insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
    };

	struct RegisterRepository : IRegisterRepository, AbstractPostgreRepository
    {
        RegisterRepository(std::shared_ptr<IAdapter> adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

        virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) override final;

        virtual ~RegisterRepository() = default;
    };

    struct RepliesRepository : IRepliesRepository, AbstractPostgreRepository
    {
        RepliesRepository(std::shared_ptr<IAdapter> adapter) { pTable = std::make_unique<PostgreTable>("users", adapter); }

        virtual std::vector<Network::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override final;
        virtual Utility::StoringReplyCodes      storeReply(const Network::ReplyInfo& rsi) override final;

        virtual ~RepliesRepository() = default;

    private:

        std::optional<pqxx::result>             insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID);
        std::optional<pqxx::result>             insertReplyIntoRepliesTable(const Network::ReplyInfo& rsi);
    };
}
