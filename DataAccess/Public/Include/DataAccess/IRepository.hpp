#pragma once

#include <string>
#include <vector>

// An interface for repository
// see https://github.com/f1nal3/Juniorgram/wiki/IRepository-scheme
// for additional details
class IRepository
{
    virtual std::vector<std::string> getAllChannelsList() = 0;
    virtual std::vector<std::string> getMessageHistoryForUser(std::string) = 0;
    virtual void storeMessages(std::vector<std::string>) = 0;
    virtual ~IRepository();
};