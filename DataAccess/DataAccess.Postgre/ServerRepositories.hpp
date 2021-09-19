#pragma once
#include "DataAccess/IServerRepositories.hpp"
#include "PostgreAdapter.hpp"
#include "UsersAmountFinder.hpp"

namespace DataAccess
{
    std::optional<pqxx::result> toPQXX(std::optional<std::any>&& optAny);

    struct ChannelsRepository : IChannelsRepository
    {
        ChannelsRepository(std::shared_ptr<IAdapter> adapter) { _adapter = adapter; }

        virtual std::vector<Network::ChannelInfo> getAllChannelsList() override final;

        virtual ~ChannelsRepository() = default;
	};

	struct LoginRepository : ILoginRepository
    {
        LoginRepository(std::shared_ptr<IAdapter> adapter) { _adapter = adapter; }

        virtual std::uint64_t loginUser(const std::string& login, const std::string& pwdHash) override final;

        virtual ~LoginRepository() = default;
    };

	struct MessagesRepository : IMessagesRepository
    {
        MessagesRepository(std::shared_ptr<IAdapter> adapter) { _adapter = adapter; }
        
        virtual std::vector<Network::MessageInfo> getMessageHistory(const std::uint64_t channelID) override final;

        virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) override final;

        virtual Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) override final;

        virtual ~MessagesRepository() = default;
    };

	struct RegisterRepository : IRegisterRepository
    {
        RegisterRepository(std::shared_ptr<IAdapter> adapter) { _adapter = adapter; }

        virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) override final;

        virtual ~RegisterRepository() = default;
    };

    struct RepliesRepository : IRepliesRepository
    {
        RepliesRepository(std::shared_ptr<IAdapter> adapter) { _adapter = adapter; }

        virtual std::vector<Network::ReplyInfo> getReplyHistory(const std::uint64_t channelID) override final;

        virtual Utility::StoringReplyCodes  storeReply(const Network::ReplyInfo& rsi) override final;

        virtual ~RepliesRepository() = default;
    };
}
