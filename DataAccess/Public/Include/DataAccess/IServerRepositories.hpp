#pragma once

#include <string>
#include <vector>

#include <Models/Primitives.hpp>
#include <Utility/Utility.hpp>

#include "IMasterRepository.hpp"

namespace DataAccess
{
/**
 * @brief The IMessagesRepository abstract subclass from IMasterRepository.
 */
struct IMessagesRepository : IMasterRepository
{
    /**
    * @brief virtual method for getting history. \
    * @param channelID - channel's id from which we want to get message history.
    * @return List of messages as vector of MessageInfo.
    */
    virtual std::vector<Models::MessageInfo> getMessageHistory(const std::uint64_t channelID) = 0;

    /**
    * @brief virtual method for storing message.
    * @param Network::MessageInfo which contains message's data for storing in repository.
    * @return The return value of the method is one of the StoringMessageCodes (enum): /
    * Storing successful - StoringMessageCodes::SUCCESS. /
    * Storing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::StoringMessageCodes storeMessage(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief virtual method for deleting message.
    * @param messageInfo.
    * @return return The return value of the method is one of the StoringMessageCodes (enum): /
    * Deleting successful - StoringMessageCodes::SUCCESS. /
    * Deleting failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::DeletingMessageCodes deleteMessage(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief virtual method for editing message.
    * @param emi as MessageInfo which contains message's data.
    * @return The return value of the method is one of the StoringMessageCodes (enum): /
    * Editing successful - StoringMessageCodes::SUCCESS. /
    * Editing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::EditingMessageCodes editMessage(const Models::MessageInfo& messageInfo) = 0;
    
    /**
    * @brief virtual method for updating message reactions
    * @param Network::MessageInfo which contains message's reaction map(id, count) with changed reaction count set to uint32_t max
    * @return The return value of the method is one of the ReactionMessageCodes (enum): /
    * Update successful - ReactionMessageCodes::SUCCESS. /
    * Update failed     - ReactionMessageCodes::FAILED.
    */
    virtual Utility::ReactionMessageCodes updateMessageReactions(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IMessagesRepository() = default;
};

/**
 * @brief The IRepliesRepository abstract subclass from IMasterRepository
 */
struct IRepliesRepository : IMasterRepository
{
    /**
     * @brief virtual method for getting history.
     */
    virtual std::vector<Models::ReplyInfo> getReplyHistory(const std::uint64_t channelID) = 0;
    
    /**
    * @brief virtual method for store reply.
    * @param replyInfo.
    * @return return The return value of the method is one of the StoringMessageCodes (enum): /
    * Storing successful - StoringMessageCodes::SUCCESS. /
    * Storing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::StoringReplyCodes storeReply(const Models::ReplyInfo& rsi) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IRepliesRepository() = default;
};

/**
 * @brief The IChannelsRepository abstract subclass from IMasterRepository
 */
struct IChannelsRepository : IMasterRepository
{
    /**
    * @brief  method for getting all channel list from the server.
    * @return channels list as vector of ChannelInfo.
    */
    virtual std::vector<Models::ChannelInfo> getAllChannelsList() = 0;

    /**
    * @brief  method for deleting channel.
    * @return special code for success/failed deleting;
    */
    virtual Utility::ChannelDeleteCode deleteChannel(const Models::ChannelDeleteInfo& channel) = 0;

    /**
    * @brief  method for creating channel.
    * @return special code for success/failed creating;
    */
    virtual Utility::ChannelCreateCodes createChannel(const Models::ChannelInfo& channel) = 0;

    /**
    * @brief  method for leaving channel.
    * @return special code for success/failed leaving;
    */
    virtual Utility::ChannelLeaveCodes leaveChannel(const Models::ChannelLeaveInfo& channel) = 0;

    /**
    * @brief  method for subscribing to channel.
    * @return special code for success/failed subscribing;
    */
    virtual Utility::ChannelSubscribingCodes subscribeToChannel(const Models::ChannelSubscriptionInfo& channel) = 0;
    
    /**
    * @brief  method for getting channel subscription list.
    * @return channels list as vector of uint64_t.
    */
    virtual std::vector<uint64_t> getChannelSubscriptionList(uint64_t userID) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IChannelsRepository() = default;
};

/**
 * @brief The IChannelsRepository abstract subclass from IMasterRepository
 */
struct IDirectMessageRepository : IMasterRepository
{
    /**
    * @brief save a direct message from *user_id* to *receiverID*
    * @param user_id Sender id
    * @param directMessageInfo receiver info
    */
    virtual Utility::DirectMessageStatus addDirectChat(uint64_t user_id, uint64_t directMessageInfo) = 0;

    ///**
    //* @brief virtual destructor.
    //*/
    //virtual ~IDirectMessageRepository() = default;
};

/**
 * @brief The IRegisterRepository abstract subclass from IMasterRepository
 */
struct IRegisterRepository : IMasterRepository
{
    /**
    * @brief Method for user registration.
    * @param RegistrationMessage which contains user data for registration.
    * @details Generation password's hash in which login is a salt. It lets us /
    * to insert different users with the same passwords.
    * @return The return value of the method is one of the RegistrationCodes (enum). /
    * Registration successful - RegistrationCodes::SUCCESS. /
    * The user already exists - RegistrationCodes::EMAIL_ALREADY_EXISTS OR /
    * RegistrationCodes::LOGIN_ALREADY_EXISTS.
    */
    virtual Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& ri) = 0;

    /**
    * @brief virtual destructor.
    */
    ~IRegisterRepository() override = default;
};

/**
 * @brief The ILoginRepository abstract subclass from IMasterRepository
 */
struct ILoginRepository : IMasterRepository
{
    /**
    * @brief attempts to login a user with the provided login by checking privided hash with the one stored.
    * @param login user login as string
    * @param pwdHash password hash
    * @return userID if provided hash is the same as stored in repository, 0 stands for failed login
    */
    virtual std::uint64_t loginUser(const Models::LoginInfo& li) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~ILoginRepository() = default;
};
}  // namespace DataAccess
