#include "SQLCipherRepository.hpp"

namespace DataAccess
{
std::string SQLCipherRepository::getRefreshToken()
{ 
    auto tokenVc = std::get<1>(SQLCipherTable("refresh_tokens").Select()->columns({"refresh_token"})->execute());

    return tokenVc.front();
}

void SQLCipherRepository::deleteRefreshToken()
{
    SQLCipherTable("refresh_tokens").Update()->fields(std::pair{"refresh_token", "0"})->execute();
}

bool SQLCipherRepository::isRefreshTokenExists()
{
    auto tokenCountVc = std::get<1>(SQLCipherTable("refresh_tokens").Select()->columns({"refresh_token"})->execute());
    if(tokenCountVc.front()[0] == '0')
    {
        return false;
    }
    return true;
}

void SQLCipherRepository::setRefreshToken(const std::string& refrToken)
{
    SQLCipherTable("refresh_tokens").Update()->fields(std::pair{"refresh_token", refrToken.c_str()})->execute();
}

suppressWarning(4100, Init) 
std::vector<std::string> SQLCipherRepository::getAllChannelsList()
{
    throw Utility::NotImplementedException("This feature has no implementation at this time!",
                                           __FILE__, __LINE__);
}

std::vector<std::string> SQLCipherRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    throw Utility::NotImplementedException("This feature has no implementation at this time!", __FILE__, __LINE__);
}

 void SQLCipherRepository::storeMessage(const Network::MessageInfo& message, const std::uint64_t channelID)
{
    throw Utility::NotImplementedException("This feature has no implementation at this time!", __FILE__, __LINE__);
}

restoreWarning

}
