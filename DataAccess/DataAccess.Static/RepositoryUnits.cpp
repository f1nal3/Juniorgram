#include "RepositoryUnits.hpp"

using namespace DataAccess;

std::string nowTimeStampStr()
{
    std::string timeStampStr(20, '\0');
    
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time  = Utility::safe_localtime(t);
    std::strftime(timeStampStr.data(), 20, "%Y-%m-%d %H:%M:%S", &time);

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

RegistrationUnit::RegistrationCodes RegistrationUnit::registerUser(const Network::RegisrtationMessage& rm) const
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

    // ID will not be autoincremented in the future. Later we are going to use postgre
    // alghorithms to create it.
    std::uint64_t userID = std::get<0>(PTable("users")
                                       .Select()
                                       ->columns({"max(id)"})
                                       ->execute())
                                       .value()[0][0].as<std::uint64_t>();
    
    std::string mainToken    = TokenUnit::instance().createToken(userID);
    std::string refreshToken = TokenUnit::instance().createToken(userID);

    std::string timeStampStr = nowTimeStampStr();

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
