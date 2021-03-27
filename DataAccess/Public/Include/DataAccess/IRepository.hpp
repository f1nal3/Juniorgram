#pragma once

#include <string>
#include <vector>

namespace DataAccess
{
// An interface for repository
// see https://github.com/f1nal3/Juniorgram/wiki/IRepository-scheme
// for additional details
class IRepository
{
    public:
    // Draft method for getting all channel list from the server.
    virtual std::vector<std::string> getAllChannelsList(std::uint64_t userID) = 0;
    // Draft method for getting history for a specific user.
    // History is all messages that a marked as "non-delivered" for this user.
    virtual std::vector<std::string> getMessageHistoryForUser(std::string) = 0;
    // Draft method for writing several messages to a repository 
    virtual void storeMessages(std::vector<std::string>) = 0;
    
    virtual ~IRepository() = default;
};
}
