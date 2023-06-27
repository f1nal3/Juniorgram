#pragma once

#include <string>
#include <vector>

#include <Models/Primitives.hpp>
#include <Utility/Utility.hpp>

#include "IMasterRepository.hpp"

namespace DataAccess
{
/**
* @class IMessagesRepository
* @brief Implements message processing on kernel side.
* @details The class specify, which information it process. It include methods
* for working with messages from database (e.g. adding, deleting, editing, getting messages). \n
* Message - is an object of MessageInfo class. As least it contains the sender
* and semantic message. \n
* The class used as base of implementation message processing for each type of database.
*/
struct IMessagesRepository : IMasterRepository
{
    /**
    * @brief Virtual method for getting history. /
    * @param channelID - channel's id from which we want to get message history.
    * @return List of messages as vector of MessageInfo.
    */
    virtual std::vector<Models::MessageInfo> getMessageHistory(const std::uint64_t channelID) = 0;

    /**
    * @brief Virtual method for storing message.
    * @param Network::MessageInfo which contains message's data for storing in repository.
    * @return The return value of the method is one of the StoringMessageCodes (enum): \n
    * Storing successful - StoringMessageCodes::SUCCESS. \n
    * Storing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::StoringMessageCodes storeMessage(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief Virtual method for deleting message.
    * @param messageInfo.
    * @return The return value of the method is one of the StoringMessageCodes (enum): \n
    * Deleting successful - StoringMessageCodes::SUCCESS. \n
    * Deleting failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::DeletingMessageCodes deleteMessage(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief Virtual method for editing message.
    * @param emi as MessageInfo which contains message's data.
    * @return The return value of the method is one of the StoringMessageCodes (enum): \n
    * Editing successful - StoringMessageCodes::SUCCESS. \n
    * Editing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::EditingMessageCodes editMessage(const Models::MessageInfo& messageInfo) = 0;
    
    /**
    * @brief Virtual method for updating message reactions
    * @param Network::MessageInfo which contains message's reaction map(id, count) with changed reaction count set to uint32_t max
    * @return The return value of the method is one of the ReactionMessageCodes (enum): \n
    * Update successful - ReactionMessageCodes::SUCCESS. \n
    * Update failed     - ReactionMessageCodes::FAILED.
    */
    virtual Utility::ReactionMessageCodes updateMessageReactions(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IMessagesRepository() = default;
};

/**
 * @class IRepliesRepository
 * @brief Implements replies processing
 * @details The class is similar to IMessagesRepository, but processing object is replies on messages
 * (object of ReplyInfo class).
 */
struct IRepliesRepository : IMasterRepository
{
    /**
    * @brief virtual method for getting history.
    */
    virtual std::vector<Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) = 0;
    
    /**
    * @brief Virtual method for store reply.
    * @param replyInfo.
    * @return The return value of the method is one of the StoringMessageCodes (enum): \n
    * Storing successful - StoringMessageCodes::SUCCESS. \n
    * Storing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::StoringReplyCodes storeReply(const Models::ReplyInfo& replyInfo) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IRepliesRepository() = default;
};

/**
 * @class IChannelsRepository
 * @brief Implements channels processing
 * @details The class is similar to IMessagesRepository, but processing object is message channels
 * (object of ChannelInfo class).
 */
struct IChannelsRepository : IMasterRepository
{
    /**
    * @brief  Virtual method for getting all channel list from the kernel.
    * @return channels list as vector of ChannelInfo.
    */
    virtual std::vector<Models::ChannelInfo> getAllChannelsList() = 0;

    /**
    * @brief  Virtual method for deleting channel.
    * @return special code for success or failed deleting.
    */
    virtual Utility::ChannelDeleteCode deleteChannel(const Models::ChannelDeleteInfo& channel) = 0;

    /**
    * @brief  Virtual method for creating channel.
    * @return special code for success or failed creating.
    */
    virtual Utility::ChannelCreateCodes createChannel(const Models::ChannelInfo& channel) = 0;

    /**
    * @brief  Virtual method for leaving channel.
    * @return special code for success or failed leaving.
    */
    virtual Utility::ChannelLeaveCodes leaveChannel(const Models::ChannelLeaveInfo& channel) = 0;

    /**
    * @brief  Virtual method for subscribing to channel.
    * @return special code for success or failed subscribing.
    */
    virtual Utility::ChannelSubscribingCodes subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) = 0;
    
    /**
    * @brief  Virtual method for getting channel subscription list.
    * @return channels list as vector of uint64_t.
    */
    virtual std::vector<uint64_t> getChannelSubscriptionList(uint64_t userID) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IChannelsRepository() = default;
};

/**
 * @class IDirectMessageRepository
 * @brief Implements personal chat processing
 * @details The class is similar to IMessagesRepository, but processing object is message in personal chat.
 */
struct IDirectMessageRepository : IMasterRepository
{
    /**
    * @brief Save a direct message from *user_id* to *receiverID*
    * @param user_id Sender id
    * @param directMessageInfo receiver info
    */
    virtual Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t directMessageInfo) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IDirectMessageRepository() = default;
};

/**
 * @class IRegisterRepository
 * @brief Implements user registration
 * @details The class is similar to IMessagesRepository, but processing object is registation information
 * (object of RegistrationInfo class).
 */
struct IRegisterRepository : IMasterRepository
{
    /**
    * @brief Method for user registration.
    * @param RegistrationMessage which contains user data for registration.
    * @details Generation password's hash in which login is a salt. It lets us
    * to insert different users with the same passwords.
    * @return The return value of the method is one of the RegistrationCodes (enum). \n
    * Registration successful - RegistrationCodes::SUCCESS. \n
    * The user already exists - RegistrationCodes::EMAIL_ALREADY_EXISTS OR
    * RegistrationCodes::LOGIN_ALREADY_EXISTS.
    */
    virtual Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) = 0;

    /**
    * @brief virtual destructor.
    */
    ~IRegisterRepository() override = default;
};

/**
 * @class ILoginRepository
 * @brief Implements user authorization
 * @details The class is similar to IMessagesRepository, but it handles logining
 * (object of LoginInfo class).
 */
struct ILoginRepository : IMasterRepository
{
    /**
    * @brief Attempts to login a user with the provided login by checking privided hash with the one stored.
    * @param login user login as string
    * @param pwdHash password hash
    * @return userID if provided hash is the same as stored in repository, 0 stands for failed login
    */
    virtual std::uint64_t loginUser(const Models::LoginInfo& loginInfo) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~ILoginRepository() = default;
};
}  // namespace DataAccess
