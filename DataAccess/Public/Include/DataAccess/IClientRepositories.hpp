#pragma once

#include <vector>
#include "IMasterRepository.hpp"

namespace DataAccess
{
struct MessageInfo;
    /**
    * @class IMessageRepository
    * @brief Client message repository
    * @details Contains methods for client side for message management.
    * This class is used for message handling on client side. Class determines
    * functions, which client can execute. Implementation of methods for each
    * type of database is individual.
    */
class IMessageRepository : public IMasterRepository
{
public:

    /**
     * @brief If message doesn't exist it will be created \
     * If message exist a new one will be ignored
     */
    virtual void addMessages(std::vector<MessageInfo>& messages) = 0;

    /**
     * @brief If message doesn't exist it will be created \
     * If message exist it will be overridden
     */
    virtual void editMessages(std::vector<MessageInfo>& messages) = 0;

    /**
     * @brief Get all messages from channel in local storage
     */
    virtual std::vector<MessageInfo> getMessages(std::uint64_t channelID) = 0;

    /**
     * @brief Get all messages in time period [&since; now]
     */
    virtual std::vector<MessageInfo> getMessageSince(std::uint64_t channelID, std::uint64_t since) = 0;

    /**
     * @brief Get &amount messages in time period before &before
     */
    virtual std::vector<MessageInfo> getMessageBefore(std::uint64_t channelID, std::uint64_t before, std::uint64_t amount) = 0;

    /**
     * @brief Remove specified messages
     */
    virtual void removeMessages(std::vector<MessageInfo>& messages) = 0;
};
}  // namespace DataAccess
