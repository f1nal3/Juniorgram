#pragma once

#include "PostgreAdapter.hpp"
#include "DataAccess/IRepository.hpp"

namespace DataAccess
{
    class PostgreRepository : public IRepository
    {
    private:
    
        std::shared_ptr<PostgreAdapter> _postgre;

    public:

        PostgreRepository();
        virtual ~PostgreRepository() = default;
    
        virtual const std::vector<std::string> getAllChannelsList() override final;
        virtual const std::vector<std::string> getMessageHistoryForUser(const std::string& UserID) override final;
        virtual void storeMessage(const MessageWrapper&) override final;
    };
}
