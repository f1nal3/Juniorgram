#pragma once

#include "DataAccess/IRepository.hpp"

namespace DataAccess
{
class PostgreRepository : public IRepository
{
    public:
    PostgreRepository() = default;
    virtual ~PostgreRepository() = default;

    virtual std::vector<std::string> getAllChannelsList() override final;
    virtual std::vector<std::string> getMessageHistoryForUser(std::string) override final;
    virtual void storeMessages(std::vector<std::string>) override final;
};
}
