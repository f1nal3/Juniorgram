#pragma once

#include <string>
#include <vector>

#include <Models/Primitives.hpp>
#include <Utility/Utility.hpp>

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
    * @return channels list as vector of ChannelInfo.
    */
    virtual std::vector<Models::ChannelInfo> getAllChannelsList() = 0;

    /**
    * @brief Draft method for getting history for a specific user. \
    * History is all messages that a marked as "non-delivered" for this user.
    * @param channelID - channel's id from which we want to get message history.
    * @return List of messages as vector of MessageInfo.
    */
    virtual std::vector<Models::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) = 0;

	/**
	* @brief draft method for getting history for a specific user.
	*/
    virtual std::vector<Models::ReplyInfo> getReplyHistoryForUser(const std::uint64_t channelID) = 0;
    
    /**  
    * @brief Method for storing message.
    * @param Network::MessageInfo which contains message's data for storing in repository.
    * @return The return value of the method is one of the StoringMessageCodes (enum): /
    * Storing successful - StoringMessageCodes::SUCCESS. /
    * Storing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::StoringMessageCodes storeMessage(const Models::MessageInfo& messageInfo) = 0;
  
    /**
    * @brief Method for store replying message.
    * @param replyInfo.
    * @return The return value of the method is one of the StoringMessageCodes (enum): /
    * Storing successful - StoringMessageCodes::SUCCESS. /
    * Storing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::StoringReplyCodes storeReply(const Models::ReplyInfo& replyInfo) = 0;
    
    /**
    * @brief Method for deleting message.
    * @param messageInfo.
    * @return The return value of the method is one of the StoringMessageCodes (enum): /
    * Deleting successful - StoringMessageCodes::SUCCESS. /
    * Deleting failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::DeletingMessageCodes deleteMessage(const Models::MessageInfo& messageInfo) = 0;

    /**
    * @brief Method for editing message
    * @param emi as MessageInfo which contains message's data.
    * @return The return value of the method is one of the StoringMessageCodes (enum): /
    * Editing successful - StoringMessageCodes::SUCCESS. /
    * Editing failed     - StoringMessageCodes::FAILED.
    */
    virtual Utility::EditingMessageCodes editMessage(const Models::MessageInfo& messageInfo) = 0;
    
    /**  
    * @brief Method for updating message reactions
    * @param Network::MessageInfo which contains message's reaction map(id, count) with changed reaction count set to uint32_t max
    * @return The return value of the method is one of the ReactionMessageCodes (enum): /
    * Update successful - ReactionMessageCodes::SUCCESS. /
    * Update failed     - ReactionMessageCodes::FAILED.
    */
    virtual Utility::ReactionMessageCodes updateMessageReactions(const Models::MessageInfo& messageInfo) = 0;

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
    virtual Utility::RegistrationCodes registerUser(const Models::RegistrationInfo& regInfo) const = 0;

    /**
    * @brief Attempts to login a user with the provided login by checking privided hash with the one stored.
    * @param login user login as string
    * @param pwdHash password hash
    * @return userID if provided hash is the same as stored in repository, 0 stands for failed login
    */
    virtual std::uint64_t loginUser(const std::string& login, const std::string& pwdHash) = 0;

    /**
    * @brief virtual destructor.
    */
    virtual ~IRepository() = default;
};
}  // namespace DataAccess
