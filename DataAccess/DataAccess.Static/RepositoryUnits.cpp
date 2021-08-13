#include "RepositoryUnits.hpp"
#include "PostgreRepository.hpp"

#include <Utility.Static/Cryptography.hpp>

std::string nowTimeStampStr()
{
    std::string timeStampStr(20, '\0');

    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time  = Utility::safe_localtime(t);
    std::strftime(timeStampStr.data(), timeStampStr.size(), "%Y-%m-%d %H:%M:%S", &time);

    return timeStampStr;
}

namespace DataAccess
{
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

std::string RegistrationUnit::getUserIdByLogin(const Network::RegistrationInfo& ri)
{ 
    auto getUserId = [&](const std::string& condition) -> std::string {
        auto result = std::get<0>(getUsersTable().Select()->columns({"Id"})->Where(condition)->execute());

        return result.value()[0][0].as<std::string>();
    };

    return getUserId("email = '" + ri.email + "'");
}

Utility::SessionCodes Sessions::SessionsManagementUnit::addSessionAfterRegistration(const std::shared_ptr<Network::Connection>& client, const std::string& refreshToken)
{
    suppressWarning(4239, Init) std::string decodedPayload =
        Coding::getBASE64DecodedValue(Utility::extractPayload(refreshToken));
    restoreWarning

    mJRepresentation = json::parse(decodedPayload);

    /*auto getAmountSessionForCurrentUserID = [](const std::string& condition) -> std::uint16_t {
        auto recordsRowAmount = std::get<0>(
            getSessionTable().Select()->columns({"COUNT(*)"})->Where(condition)->execute());

        return recordsRowAmount.value()[0][0].as<std::uint16_t>();
    };

    if (getAmountSessionForCurrentUserID("user_id = '" + std::string(mJRepresentation["id"]) + "'") > mMaxSessionForCurrentUser)
    {
        return Utility::SessionCodes::TOO_MANY_SESSIONS_FOR_CURRENT_USER;
    }*/

    std::tuple sessionData
    {
        std::pair{"user_id", std::string(mJRepresentation["id"])},
        std::pair{"ip", std::string(mJRepresentation["ip"])},  
        std::pair{"os", std::string(mJRepresentation["os"])},
        std::pair{"expire", Utility::WithoutQuotes("to_timestamp(" + std::string(mJRepresentation["exp"]) + ")::timestamp")},
        std::pair{"created_at", Utility::WithoutQuotes("to_timestamp(" + std::string(mJRepresentation["iat"]) + ")::timestamp")},
        std::pair{"updated_at", Utility::WithoutQuotes("to_timestamp(" + std::string(mJRepresentation["upd"]) + ")::timestamp")},
        std::pair{"fingerprint", std::string(mJRepresentation["sub"])},
        std::pair{"refresh_token", Coding::getHexCodedValue(refreshToken)},
        std::pair{"signature_key", Coding::getHexCodedValue(std::string(client->getSignerAndVerifierInstance()->getPrivateKey()))},
        std::pair{"signature_verification_key", Coding::getHexCodedValue(std::string(client->getSignerAndVerifierInstance()->getPublicKey()))},
    };

    getSessionTable().Insert()->columns(sessionData)->execute();

    return Utility::SessionCodes::SUCCESS;
}

void Sessions::SessionsManagementUnit::revokeSession(const std::string& refreshToken)
{
    getSessionTable().getAdapter()->query(
        "INSERT INTO revoked_sessions SELECT id, user_id, ip, expire, fingerprint, refresh_token "
        "FROM sessions WHERE refresh_token = '" +
        refreshToken + "'");

    getSessionTable().Delete()->Where("refresh_token = '" + refreshToken + "'")->execute();
}

}  // namespace DataAccess