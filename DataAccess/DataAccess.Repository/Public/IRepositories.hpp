#pragma once

#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>
#include <string>
#include <vector>

namespace DataAccess
{

/**
 * @brief The IMasterRepository base abstract class
 */
struct IMasterRepository
{
    /// Default virtual destructor
    virtual ~IMasterRepository() = default;
};

/**
 * @brief The IMessagesRepository abstract subclass from IMasterRepository
 */
struct IMessagesRepository : IMasterRepository
{
    /// Virtual method for geting Message History
    virtual std::vector<Network::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) = 0;

    /// Virtual method for store Message
    virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& mi) = 0;

    /// Virtual method for delete Message
    virtual Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) = 0;

    /// Default virtual destructor
    virtual ~IMessagesRepository() = default;
};

/**
 * @brief The IRepliesRepository abstract subclass from IMasterRepository
 */
struct IRepliesRepository : IMasterRepository
{
    /// Virtual method for geting Reply History
    virtual std::vector<Network::ReplyInfo> getReplyHistoryForUser(const std::uint64_t channelID) = 0;

    /// Default virtual destructor
    virtual ~IRepliesRepository() = default;
};

/**
 * @brief The IChannelsRepository abstract subclass from IMasterRepository
 */
struct IChannelsRepository : IMasterRepository
{
    /// Virtual method for geting channgel List
    virtual std::vector<Network::ChannelInfo> getAllChannelsList() = 0;

    /// Default virtual destructor
    virtual ~IChannelsRepository() = default;
};

/**
 * @brief The IRegisterRepository abstract subclass from IMasterRepository
 */
struct IRegisterRepository : IMasterRepository
{
    /// Virtual method for register User
    virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) = 0;

    /// Default virtual destructor
    virtual ~IRegisterRepository() = default;
};

/**
 * @brief The ILoginRepository abstract subclass from IMasterRepository
 */
struct ILoginRepository : IMasterRepository
{
    /// Virtual method for login User
    virtual std::uint64_t loginUser(const std::string& login, std::string& pwdHash) = 0;

    /// Default virtual destructor
    virtual ~ILoginRepository() = default;
};

}  // namespace DataAccess
