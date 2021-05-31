#pragma once

#include <Network/Primitives.hpp>

#include <ctime>
#include <iostream>
#include <random>

#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

#include "DataAccess/IRepository.hpp"
#include "DatabaseAbstractionLayout.hpp"

namespace DataAccess
{
class PostgreRepository : public IRepository
{
public:
    PostgreRepository() = default;
    virtual ~PostgreRepository() = default;

    virtual std::vector<std::string> getAllChannelsList() override final;
    virtual std::vector<std::string> getMessageHistoryForUser(const std::uint64_t channelID) override final;
    virtual void storeMessage(const Network::MessageInfo& message, const std::uint64_t channleID) override final;
};
}  // namespace DataAccess

class TokenUnit
{
private:
    const std::uint16_t TOKEN_LENGTH = 32;

    TokenUnit() = default;
    // TokenUnit(const TokenUnit&) = delete;
    // void operator=(const TokenUnit& t) = delete;
public:
    static TokenUnit instance()
    {
        static TokenUnit token;
        return token;
    }

    inline std::uint16_t getTokenLength() const { return TOKEN_LENGTH; }

    std::string createToken(const std::uint64_t userID) const
    {
        std::string userIDstr = std::to_string(userID);

        std::string possibleCharacters =
            "!@#$%^&*()-_=<>ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        std::mt19937 generator(std::random_device{}());

        std::shuffle(possibleCharacters.begin(), possibleCharacters.end(), generator);

        for (size_t i = 0; i < userIDstr.size(); ++i)
        {
            possibleCharacters[i] = userIDstr[i];
        }

        return possibleCharacters.substr(0, TOKEN_LENGTH);
    }
};

using namespace DataAccess;

class RegistrationUnit
{
private:
    RegistrationUnit() = default;

public:
    enum class RegistrationCodes : std::uint8_t
    {
        EMAIL_ALREADY_EXISTS,
        LOGIN_ALREADY_EXISTS,
        SUCCESS,
    };

    inline static RegistrationUnit instance()
    {
        static RegistrationUnit registartor;
        return registartor;
    }

public:
    RegistrationUnit(const RegistrationUnit&) = default;
    ~RegistrationUnit()                       = default;

    RegistrationCodes registerUser(const Network::RegisrtationMessage& rm)
    {
        // Check on existing of login and email in repository.
        auto getUsersAmount = [&](const std::string& condition) -> std::size_t 
        {
            auto recordsRowAmount = std::get<0>(PTable("users")
                                                .Select()
                                                ->columns({"COUNT(*)"})
                                                ->Where(condition)
                                                ->execute());

            return recordsRowAmount.value()[0][0].as<std::size_t>();
        };

        if (getUsersAmount("email = '" + std::string(rm.email) + "'") > 0)
        {
            return RegistrationCodes::EMAIL_ALREADY_EXISTS;
        }
        if (getUsersAmount("login = '" + std::string(rm.login) + "'"))
        {
            return RegistrationCodes::LOGIN_ALREADY_EXISTS;
        }

        std::tuple userData
        {
            std::pair{"email", rm.email}, 
            std::pair{"login", rm.login},
            std::pair{"password_hash", rm.password}
        };
        PTable("users").Insert()->columns(userData)->execute();

        std::uint64_t userID = std::get<0>(PTable("users")
                                           .Select()
                                           ->columns({"max(id)"})
                                           ->execute())
                                           .value()[0][0].as<std::uint64_t>();

        std::string mainToken    = TokenUnit::instance().createToken(userID);
        std::string refreshToken = TokenUnit::instance().createToken(userID);

        char timeStampStr[20];
        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm time  = Utility::safe_localtime(t);
        std::strftime(timeStampStr, 20, "%Y-%m-%d %H:%M:%S", &time);

        std::tuple tokens
        {
            std::pair{"user_id", userID},
            std::pair{"token", mainToken},
            std::pair{"refresh_token", refreshToken},
            std::pair{"token_receipt_time", timeStampStr},
        };
        PTable("user_tokens").Insert()->columns(tokens)->execute();

        return RegistrationCodes::SUCCESS;
    }
};
