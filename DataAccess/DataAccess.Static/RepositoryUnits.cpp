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

Utility::SessionCodes SessionsManagementUnit::addSessionAfterRegistration(const std::string& refreshToken)
{
    suppressWarning(4239, Init)
    std::string decodedPayload = Coding::getBASE64DecodedValue(Utility::extractPayload(refreshToken));
    restoreWarning

    mJRepresentation = json::parse(decodedPayload);

    std::tuple sessionData
    {
        std::pair{"user_id", std::string(mJRepresentation["id"])},
        std::pair{"ip", std::string(mJRepresentation["ip"])},  
        std::pair{"os", std::string(mJRepresentation["os"])},
        std::pair{"expire", Utility::WithoutQuotes("to_timestamp(" + std::string(mJRepresentation["exp"]) + ")::timestamp")},
        std::pair{"created_at", Utility::WithoutQuotes("to_timestamp(" + std::string(mJRepresentation["iat"]) + ")::timestamp")},
        std::pair{"updated_at", Utility::WithoutQuotes("to_timestamp(" + std::string(mJRepresentation["upd"]) + ")::timestamp")},
        std::pair{"fingerprint", std::string(mJRepresentation["sub"])},
        std::pair{"refresh_token", std::string(refreshToken)},
        std::pair{"signature_verification_key", std::string("verif")}
    };

    getSessionTable().Insert()->columns(sessionData)->execute();
    
    //std::string hexedToken = (std::get<0>(getSessionTable().Select()->columns({std::string("token")})->execute())).value()[0][0].as<std::string>();

   // std::string decodedToken;

   // CryptoPP::StringSource ss(hexedToken, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(decodedToken)));                                                       
   //
   // std::string m =
   //    "eyJhbGciOiJFQ0RTQS9FTVNBMShTSEEtMjU2KSIsInR5cCI6IkpXVCJ9."
   //    "eyJleHAiOiIxNjI3NzQ3NDAxIiwiaWF0IjoiMTYyNzY2MTAwMSIsImlkIjoiICIsImlwIjoiMTI3LjAuMC4xIiwianR"
   //    "pIjoicmVmcmVzaF90b2tlbiIsIm9zIjoiV2luZG93cyIsInBydCI6ImpnLmNsaWVudCIsInN1YiI6IlJiNmgyQTNYaD"
   //    "RqaVhsa3pJajJTcWgwblJqdz0iLCJ1cGQiOiIxNjI3NjYxMDAxIn0=.ƒhªÂÔt¿ŒûYú/‘5*Š¢OÓ¹ö0Ä÷Ä	"
   //    "µÅ›[Ô´t$j¤Ð¹a”Pyù[–Ø¯…Êp)yõÙFyâ‰‰)";


   //if (decodedToken == m)
   //{
   //    std::cout << "suceess";
   //}




    return Utility::SessionCodes::SUCCESS;
}

}  // namespace DataAccess