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

Utility::RegistrationCodes RegistrationUnit::registerUser(const Network::RegistrationInfo& ri) const
{
    auto getUsersAmount = [&](const std::string& condition) -> std::uint16_t 
    {
        auto recordsRowAmount = std::get<0>(getUsersTable().Select()
                                               ->columns({"COUNT(*)"})
                                               ->Where(condition)
                                               ->execute());
											   
        return recordsRowAmount.value()[0][0].as<std::uint16_t>();
    };

    // Check on existing of login and email in repository.
    if (getUsersAmount("email = '" + ri.email + "'") > 0)
    {
        return Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS;
    }
    if (getUsersAmount("login = '" + ri.login + "'") > 0)
    {
        return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
    }

    // Data preperaion for new user inserting.
    std::tuple userData
    {
        std::pair{"email", ri.email}, 
        std::pair{"login", ri.login},
        std::pair{"password_hash", ri.passwordHash}
    };
    // Insert new user.
    getUsersTable().Insert()->columns(userData)->execute();

    return Utility::RegistrationCodes::SUCCESS;
}
