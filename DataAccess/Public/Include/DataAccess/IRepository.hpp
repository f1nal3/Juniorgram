#pragma once

#include <string>
#include <vector>

namespace DataAccess
{
    /** @class IRepository
     *  @brief An interface for repository
     *  @details see https://github.com/f1nal3/Juniorgram/wiki/IRepository-scheme for additional details
     */
class IRepository
{
    public:
    /**
     * @brief Draft method for getting all channel list from the server.
     * @return channels list as vector of strings.
     */
    virtual std::vector<std::string> getAllChannelsList() = 0;
    /**
     * @brief Draft method for getting history for a specific user. \
     * History is all messages that a marked as "non-delivered" for this user.
     * @param UserID as a string
     * @return List of messages as vector of strings.
     */
    virtual std::vector<std::string> getMessageHistoryForUser(const std::string& UserID) = 0;
    /**
     * @brief stores messages in repository
     * @param MessagesList as vector of strings.
     */
    virtual void storeMessages(const std::vector<std::string>& MessagesList) = 0;

    /**
     * @brief Virtual dtor.
     */
    virtual ~IRepository() = default;
};
}
