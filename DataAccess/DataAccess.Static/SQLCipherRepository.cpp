#include "SQLCipherRepository.hpp"

namespace DataAccess
{
std::string SQLCipherRepository::getRefreshToken()
{ 
    auto tokenVc =
        std::get<1>(LTable("refresh_tokens").Select()->columns({"refresh_token"})->execute());

    return {tokenVc.front()};
}

bool SQLCipherRepository::isRefreshTokenExists()
{
    auto tokenCountVc = std::get<1>(
        LTable("refresh_tokens").Select()->columns({"count(refresh_token)"})->execute());

    return std::stoi(tokenCountVc.front());
}

std::vector<std::string> SQLCipherRepository::getAllChannelsList()
{
    throw Utility::NotImplementedException("This feature has no implementation at this time!", __FILE__, __LINE__);
    return {};
}

std::vector<std::string> SQLCipherRepository::getMessageHistoryForUser(
    const std::uint64_t channelID)
{
    throw Utility::NotImplementedException("This feature has no implementation at this time!", __FILE__, __LINE__);
    return {};
}

void SQLCipherRepository::storeMessage(const Network::MessageInfo& message, const std::uint64_t channelID)
{
    throw Utility::NotImplementedException("This feature has no implementation at this time!", __FILE__, __LINE__);
}

}
