#pragma once

#include <Network/Primitives.hpp>

#include "DataAccess/IRepository.hpp"
#include "PostgreAbstractionLayout.hpp"

namespace DataAccess
{
class PostgreRepository : public IRepository
{
public:
    PostgreRepository() = default;
    virtual ~PostgreRepository() = default;

    virtual std::vector<std::string> getAllChannelsList() override final;
    virtual std::vector<std::string> getMessageHistoryForUser(
        const std::string& userID /*, const unsigned channleID*/) override final;
    virtual void storeMessage(
        const Network::UserMessage& /*, const unsigned channleID*/) override final;
};
}  // namespace DataAccess
