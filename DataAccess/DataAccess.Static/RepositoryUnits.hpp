#pragma once
#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>
#include <ctime>
#include <memory>
#include <string>

#include "PostgreRepository.hpp"
#include "UsersAmountFunctions.hpp"

std::string nowTimeStampStr();

/**  @class RegistrationUnit.
 *  @brief RegistrationUnit for user's registration.
 */
class RegistrationUnit
{
protected:
    using Table = DataAccess::PostgreTable;

    RegistrationUnit(DataAccess::PostgreTable* pt) : pTable(pt) {}

private:
    std::unique_ptr<Table> pTable;

public:
    RegistrationUnit() : pTable(new Table("users")) {}

    virtual ~RegistrationUnit() = default;

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
};
