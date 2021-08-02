#pragma once
#include <Network/Primitives.hpp>
#include <string>
#include <vector>
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
     * @return channels list as vector of strings.
     */
    virtual std::vector<Network::ChannelInfo> getAllChannelsList() = 0;

    /**
     * @brief Draft method for getting history for a specific user. \
     * History is all messages that a marked as "non-delivered" for this user.
     * @param UserID as a string
     * @return List of messages as vector of strings.
     */
    virtual std::vector<Network::MessageInfo> getMessageHistoryForUser(const std::uint64_t channelID) = 0;
    
    /**
     * @brief Method for storing user's message in repository
     * @param message as Network::MessageInfo.
     */
    virtual Utility::StoringMessageCodes storeMessage(const Network::MessageInfo& msi) = 0;
    
    /**  @brief Method for user registration.
     *   @params RegistrationMessage which contains user data for registration.
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
