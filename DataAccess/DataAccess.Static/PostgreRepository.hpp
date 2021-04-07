#pragma once

#include "PostgreAdapter.hpp"
#include "DataAccess/IRepository.hpp"
#include <Network/Message.hpp>

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
        virtual std::vector<std::string> getMessageHistoryForUser(std::string) override final;
        virtual void storeMessage(Network::Message message) override final;
    
    };
}
