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

    virtual std::vector<std::string> getAllChannelsList() override final;
    virtual std::vector<std::string> getMessageHistoryForUser(const std::uint64_t channelID) override final;
    virtual void storeMessage(const Network::MessageStoringInfo& message) override final;
    virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const override final;

private:
    void insertMessageIntoMessagesTable(const Network::MessageStoringInfo& msi);
    void insertIDsIntoChannelMessagesTable(const std::uint64_t chinnelID, const std::uint64_t messageID);
    void insertIDIntoMessageReactionsTable(const std::uint64_t messageID);
};
}  // namespace DataAccess
