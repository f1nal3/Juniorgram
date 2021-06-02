#pragma once
#include <ctime>
#include <random>
#include <string>

#include <Network/Primitives.hpp>
#include "DataAccess/IRepository.hpp"
#include "DatabaseAbstractionLayout.hpp"

std::string nowTimeStampStr();

class TokenUnit
{
private:
    const std::uint16_t TOKEN_LENGTH = 32;

    TokenUnit() = default;

public:
    TokenUnit(const TokenUnit&) = delete;
    void operator=(const TokenUnit&) = delete;

    static TokenUnit& instance()
    {
        static TokenUnit token;
        return token;
    }

    inline std::uint16_t getTokenLength() const { return TOKEN_LENGTH; }

    std::string createToken(const std::uint64_t userID) const;
};

using namespace DataAccess;

class RegistrationUnit
{
private:
    RegistrationUnit() = default;

public:
    RegistrationUnit(const RegistrationUnit&) = delete;
    RegistrationUnit& operator=(RegistrationUnit&) = delete;

    enum class RegistrationCodes : std::uint8_t
    {
        EMAIL_ALREADY_EXISTS,
        LOGIN_ALREADY_EXISTS,
        SUCCESS,
    };

    inline static RegistrationUnit& instance()
    {
        static RegistrationUnit registartor;
        return registartor;
    }

public:
    ~RegistrationUnit() = default;

    RegistrationCodes registerUser(const Network::RegisrtationMessage& rm) const;
};
