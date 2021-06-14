#include "RepositoryUnits.hpp"
#include "PostgreRepository.hpp"

using namespace DataAccess;

std::string nowTimeStampStr()
{
    std::string timeStampStr(20, '\0');
    
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time  = Utility::safe_localtime(t);
    std::strftime(timeStampStr.data(), timeStampStr.size(), "%Y-%m-%d %H:%M:%S", &time);

    return timeStampStr;
}

std::string TokenUnit::createToken(const std::uint64_t userID) const
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

Utility::RegistrationCodes RegistrationUnit::registerUser(
    const Network::RegistrationInfo& rm) const
{
    auto getUsersAmount = [&](const std::string& condition) -> std::uint16_t 
    {
        auto recordsRowAmount = std::get<0>(PTable("users")
                                            .Select()
                                            ->columns({"COUNT(*)"})
                                            ->Where(condition)
                                            ->execute());

        return recordsRowAmount.value()[0][0].as<std::uint16_t>();
    };

    // Check on existing of login and email in repository.
    if (getUsersAmount("email = '" + rm.email + "'") > 0)
    {
        return Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS;
    }
    if (getUsersAmount("login = '" + rm.login + "'"))
    {
        return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
    }

    std::tuple userData
    {
        std::pair{"email", rm.email},
        std::pair{"login", rm.login},
        std::pair{"password_hash", rm.password}
    };
    PTable("users").Insert()->columns(userData)->execute();

    return Utility::RegistrationCodes::SUCCESS;
}
