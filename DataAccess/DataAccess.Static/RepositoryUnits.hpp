#pragma once
#include <ctime>
#include <random>
#include <string>
#include <nlohmann/json.hpp>

#include "PostgreRepository.hpp"

#include <Network.Static/Connection.hpp>
#include <Network.Static/Primitives.hpp>


#include <Utility/Utility.hpp>

std::string nowTimeStampStr();

namespace DataAccess
{
/**  @class RegistrationUnit.
 *  @brief RegistrationUnit for user's registration.
 */
class RegistrationUnit
{
private:
    RegistrationUnit() = default;

    inline static DataAccess::PostgreTable& getUsersTable()
    {
        static DataAccess::PostgreTable pt("users");
        return pt;
    }

public:
    RegistrationUnit(const RegistrationUnit&) = delete;
    RegistrationUnit& operator=(RegistrationUnit&) = delete;
    ~RegistrationUnit()                            = default;

    /**  @brief Method for getting of single RegistrationUnit instance.
     *   @details This instance need for gaining access to /
     *   other class members from outside.
     *   @return RegistrationUnit - single class object.
     */
    inline static RegistrationUnit& instance()
    {
        static RegistrationUnit registrator;
        return registrator;
    }

    /**  @brief Method for user registration.
     *   @params RegistrationMessage which contains user data for registration.
     *   @details Generation password's hash which are based on login. It lets us /
     *   to insert different users with the same passwords.
     *   @return The return value of the method is one of the RegistrationCodes (enum). /
     *   Registration successful - RegistrationCodes::SUCCESS. /
     *   The user already exists - RegistrationCodes::EMAIL_ALREADY_EXISTS OR /
     *   RegistrationCodes::LOGIN_ALREADY_EXISTS.
     */
    Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& ri) const;

    std::string getUserIdByLogin(const Network::RegistrationInfo& ri);
};

namespace Sessions
{
    class SessionsManagementUnit
    {
    private:
        using json = nlohmann::json;

        SessionsManagementUnit() = default;

        inline static DataAccess::PostgreTable& getSessionTable()
        {
            static DataAccess::PostgreTable pt("sessions");
            return pt;
        }

        json mJRepresentation;

    public:
        SessionsManagementUnit(const SessionsManagementUnit&) = delete;
        SessionsManagementUnit& operator=(SessionsManagementUnit&) = delete;
        ~SessionsManagementUnit()                                  = default;

        inline static SessionsManagementUnit& instance()
        {
            static SessionsManagementUnit sessionManager;
            return sessionManager;
        }

        const unsigned short mMaxSessionForCurrentUser = 5;
        Utility::SessionCodes addSessionAfterRegistration(
            const std::shared_ptr<Network::Connection>& client, const std::string& refreshToken);
    };
}
}