#pragma once

#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>
#include <string>
#include <vector>

#include "IMasterRepository.hpp"

namespace DataAccess
{
/**
 * @brief The IMessagesRepository abstract subclass from IMasterRepository
 */
struct IMessagesRepository : IMasterRepository
{
    /// Virtual method for geting Message History
    virtual std::vector<Network::MessageInfo> getMessageHistory(const std::uint64_t channelID) = 0;

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
    virtual std::vector<Network::ReplyInfo> getReplyHistory(const std::uint64_t channelID) = 0;

    /// Virtual method for inserting Reply message
    virtual Utility::StoringReplyCodes      storeReply(const Network::ReplyInfo& rsi) = 0;

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
    /// Virtual method for delete channel
    virtual Utility::ChannelDeleteCode        deleteChannel(const Network::ChannelDeleteInfo& channel) = 0;

    /// Virtual method for create channel
    virtual Utility::ChannelCreateCodes createChannel(const Network::ChannelInfo& channel) = 0;

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
    virtual std::uint64_t loginUser(const Network::LoginInfo& li/*std::string& login, const std::string& pwdHash*/) = 0;

    /// Default virtual destructor
    virtual ~ILoginRepository() = default;
};

}  // namespace DataAccess
