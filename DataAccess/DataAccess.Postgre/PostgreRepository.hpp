#pragma once

#include <Network/Primitives.hpp>

#include <iostream>

#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

#include "DataAccess/IRepository.hpp"
#include "PostgreTable.hpp"

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

    std::vector<std::string> getAllChannelsList() override final;
    std::vector<std::string> getMessageHistoryForUser(const std::uint64_t channelID) override final;
    Utility::StoringMessageCodes storeMessage(const Network::MessageStoringInfo& msi) override final;
    Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const override final;

private:
    std::optional<pqxx::result> insertMessageIntoMessagesTable(const Network::MessageStoringInfo& msi);
    std::optional<pqxx::result> insertIDsIntoChannelMessagesTable(const std::uint64_t chinnelID, const std::uint64_t messageID);
    std::optional<pqxx::result> insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};
}  // namespace DataAccess
