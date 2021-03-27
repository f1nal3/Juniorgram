#pragma once

#include "PostgreAdapter.hpp"
#include "DataAccess/IRepository.hpp"

namespace DataAccess
{
    class PostgreRepository : public IRepository
    {
    private:
    
        std::shared_ptr<PostgreAdapter> mp_dataBaseInstance;

    public:

        PostgreRepository();
        virtual ~PostgreRepository() = default;
    
        virtual std::vector<std::string> getAllChannelsList(std::uint64_t userID) override final;
        virtual std::vector<std::string> getMessageHistoryForUser(std::string) override final;
        virtual void storeMessages(std::vector<std::string>) override final;
    
    };
}
