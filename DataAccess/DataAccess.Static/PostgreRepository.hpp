#pragma once

#include <Network/Primitives.hpp>

#include <ctime>
#include <iostream>

#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

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
    virtual std::vector<std::string> getMessageHistoryForUser(const std::uint64_t channelID) override final;
    virtual void storeMessage(const Network::MessageInfo& message, const std::uint64_t channleID) override final;
};
}  // namespace DataAccess
