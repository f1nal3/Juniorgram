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
    SQLCipherTable("refresh_tokens").Delete();
}

bool SQLCipherRepository::isRefreshTokenExists()
{
    auto tokenCountVc = std::get<1>(SQLCipherTable("refresh_tokens").Select()->columns({"count(refresh_token)"})->execute());

    return std::stoi(tokenCountVc.front());
}

void SQLCipherRepository::setRefreshToken(const std::string& refrToken)
{
    SQLCipherTable("refresh_tokens").Insert()->columns(std::pair{"refresh_token", refrToken.c_str()});
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
