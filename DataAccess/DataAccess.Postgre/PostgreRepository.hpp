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
    virtual void storeMessage(const Network::MessageInfo& message, const std::uint64_t channleID) override final;
    virtual std::uint64_t loginUser(const std::string& login, const std::string& pwdHash) override final;
    virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const override final;
};
}  // namespace DataAccess
