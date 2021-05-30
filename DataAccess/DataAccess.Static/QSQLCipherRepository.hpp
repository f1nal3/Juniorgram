#pragma once

#include <Network/Primitives.hpp>
#include <ctime>
#include <iostream>

#include "DataAccess/IRepository.hpp"
#include "DatabaseAbstractionLayout.hpp"
#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

namespace DataAccess
{
class QSQLCipherRepository : public IRepository
{
public:
    QSQLCipherRepository()          = default;
    virtual ~QSQLCipherRepository() = default;


private:

    virtual std::vector<std::string> getAllChannelsList() override final;
    virtual std::vector<std::string> getMessageHistoryForUser(
        const std::uint64_t channelID) override final;
    virtual void storeMessage(const Network::MessageInfo& message,
                              const std::uint64_t channleID) override final;
};
}  // namespace DataAccess
