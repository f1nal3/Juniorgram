#pragma once

#include <DataAccess/IRepository.hpp>
#include <Network/Primitives.hpp>
#include <iostream>

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

    std::vector<Network::ChannelInfo> getAllChannelsList() final;
    std::vector<Network::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) final;
    std::uint64_t                     loginUser(const std::string& login, const std::string& pwdHash) final;
    Utility::StoringMessageCodes      storeMessage(const Network::MessageInfo& mi) final;
    Utility::RegistrationCodes        registerUser(const Network::RegistrationInfo& ri) const final;
    Utility::DeletingMessageCodes     deleteMessage(const Network::MessageInfo& mi) final;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Network::MessageInfo& msi);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t chinnelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};
}  // namespace DataAccess
