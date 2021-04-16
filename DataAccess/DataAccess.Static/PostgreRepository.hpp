#pragma once

#include "PostgreAdapter.hpp"
#include "DataAccess/IRepository.hpp"
#include <Network/Primitives.hpp>

namespace DataAccess
{
    class PostgreRepository : public IRepository
    {
    private:
    
        std::shared_ptr<PostgreAdapter> _postgre;

    public:

        PostgreRepository();
        virtual ~PostgreRepository() = default;
    
        virtual std::vector<std::string> getAllChannelsList() override final;
        virtual std::vector<std::string> getMessageHistoryForUser(const std::string& UserID) override final;
        virtual void storeMessage(const Network::UserMessage&) override final;
    };
}
