#pragma once

#include <Network/Primitives.hpp>
#include <iostream>

#include "DataAccess/IRepository.hpp"
#include "PostgreTable.hpp"
#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

namespace DataAccess
{
class PostgreRepository : public IRepository
{
protected:
    // This constructor lets us test all this functions.
    PostgreRepository(PostgreTable* pt) : pTable(pt) {}

public:
    std::unique_ptr<PostgreTable> pTable;

    PostgreRepository() : pTable(new PostgreTable{""}) {}

    virtual ~PostgreRepository() = default;

    std::vector<Network::ChannelInfo> getAllChannelsList() override final;
    std::vector<Network::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) override final;
    std::vector<Network::ReplyInfo>   getReplyHistoryForUser(const std::uint64_t channelID) override final;
    std::uint64_t                     loginUser(const std::string& login, const std::string& pwdHash) override final;
    Utility::StoringMessageCodes      storeMessage(const Network::MessageInfo& mi) override final;
    Utility::StoringReplyCodes        storeReply(const Network::ReplyInfo& rsi) override final;
    Utility::RegistrationCodes        registerUser(const Network::RegistrationInfo& ri) const override final;
    Utility::DeletingMessageCodes     deleteMessage(const Network::MessageInfo& mi) override final;
    Utility::DeletingChannelCodes     deleteChannel(const Network::MessageInfo& mi) override final;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Network::MessageInfo& msi);
    std::optional<pqxx::result> insertReplyIntoRepliesTable(const Network::ReplyInfo& rsi);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t channelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDsIntoChannelRepliesTable(const std::uint64_t channelID, const std::uint64_t replyID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};
}  // namespace DataAccess
