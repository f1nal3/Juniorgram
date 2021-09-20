#pragma once
#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>
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
     * @return channels list as vector of ChannelInfo.
     */
    virtual std::vector<Network::ChannelInfo> getAllChannelsList() = 0;

    /**
     * @brief Draft method for getting history for a specific user. \
     * History is all messages that a marked as "non-delivered" for this user.
     * @param channelID - channel's id from which we want to get message history.
     * @return List of messages as vector of MessageInfo.
     */
    virtual std::vector<Network::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) = 0;

	  /**
	   * @brief Draft method for getting history for a specific user.
	   */
	  virtual std::vector<Network::ReplyInfo>   getReplyHistoryForUser(const std::uint64_t channelID) = 0;
    
    /**  @brief Method for storing message.
     *   @param Network::MessageInfo which contains message's data for storing in repository.
     *   @return The return value of the method is one of the StoringMessageCodes (enum): /
     *   Storing successful - StoringMessageCodes::SUCCESS. /
     *   Storing failed     - StoringMessageCodes::FAILED.
     */
    virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& msi) = 0;
  
    /**
     * @brief storeReply
     * @param rsi
     * @return
     */
    virtual Utility::StoringReplyCodes storeReply(const Network::ReplyInfo& rsi) = 0;
    
    /**
     * @brief deleteMessage
     * @param mi
     * @return
     */
    virtual Utility::DeletingMessageCodes deleteMessage(const Network::MessageInfo& mi) = 0;

    virtual Utility::DeletingChannelCodes deleteChannel(const Network::MessageInfo& mi) = 0;

    /**  @brief Method for user registration.
     *   @param RegistrationMessage which contains user data for registration.
     *   @details Generation password's hash in which login is a salt. It lets us /
     *   to insert different users with the same passwords.
     *   @return The return value of the method is one of the RegistrationCodes (enum). /
     *   Registration successful - RegistrationCodes::SUCCESS. /
     *   The user already exists - RegistrationCodes::EMAIL_ALREADY_EXISTS OR /
     *   RegistrationCodes::LOGIN_ALREADY_EXISTS.
     */
    virtual Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const = 0;
    /**
     * @brief attempts to login a user with the provided login by checking privided hash with the one stored.
     * @param login user login as string
     * @param pwdHash password hash
     * @return userID if provided hash is the same as stored in repository, 0 stands for failed login
     */
    virtual std::uint64_t loginUser(const std::string& login, const std::string& pwdHash) = 0;
    /**
     * @brief Virtual dtor.
     */
    virtual ~IRepository() = default;
};
}  // namespace DataAccess
