#pragma once
#include <ctime>
#include <random>
#include <string>

#include <Network/Primitives.hpp>
#include <Utility/Utility.hpp>

std::string nowTimeStampStr();

/**  @class TokenUnit.
 *  @brief TokenUnit for work with tokens.
 */
class TokenUnit
{
private:
    const std::uint16_t TOKEN_LENGTH = 32;

    TokenUnit() = default;

public:
    TokenUnit(const TokenUnit&)            = delete;
    TokenUnit& operator=(const TokenUnit&) = delete;
    ~TokenUnit()                           = default;

    /**  @brief Method for getting of single TokenUnit instance.
     *   @details This instance need for gaining access to /
     *   other class members from outside.
     *   @return TokenUnit - single class object.
     */
    inline static TokenUnit& instance()
    {
        static TokenUnit token;
        return token;
    }
    
    /**  @brief Method for getting of token length.
    *   @return std::uint16_t token.
    */
    inline std::uint16_t getTokenLength() const { return TOKEN_LENGTH; }

    /**  @brief DRAFT method of token creating for user.
     *   @Details Tokens need for authorization and protection /
     *   against unauthorized access.
     *   @return std::string token.
     */
    std::string createToken(const std::uint64_t userID) const;
};

/**  @class RegistrationUnit.
 *  @brief RegistrationUnit for user's registration.
 */
class RegistrationUnit
{
private:
    RegistrationUnit() = default;

public:
    RegistrationUnit(const RegistrationUnit&)      = delete;
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
     *   @return The return value of the method is one of the RegistrationCodes (enum). /
     *   Registration successful - RegistrationCodes::SUCCESS. / 
     *   The user already exists - RegistrationCodes::EMAIL_ALREADY_EXISTS OR /
     *   RegistrationCodes::LOGIN_ALREADY_EXISTS.
     */
    Utility::RegistrationCodes registerUser(const Network::RegistrationInfo& rm) const;
};
