#include "PostgreRepository.hpp"

#include <ctime>
#include <iostream>

#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

using namespace DataAccess;

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;

    auto channelListRow = Table("Channels").Select()->columns({"ChannelName"})->execute();

    for (auto item : channelListRow.value())
    {
        std::cout << item.at(0).c_str() << '\n';
        result.emplace_back(std::string(item.at(0).c_str()));
    }

    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(const std::string& userID /*, const unsigned channleID*/)
{
    std::vector<std::string> result;

    auto messageHistoryRow = 
        Table("ChannelMsgs").Select()->columns({"Msg"})->where("SenderID = " + userID)/*->And("ChannelID = " + channelID)*/->execute();

    for (auto message : messageHistoryRow.value())
    {
        std::cout << message.at(0).c_str() << '\n';
        result.emplace_back(std::string(message.at(0).c_str()));
    }

    return result;
}

void PostgreRepository::storeMessage(const Network::UserMessage& message /*, const unsigned channleID*/)
{
    std::string timeStr(30, '\0');

    std::time_t t = std::chrono::system_clock::to_time_t(message.getTimestamp());
    std::tm time  = Utility::safe_localtime(t);
    std::strftime(&timeStr[0], timeStr.size(), "%Y-%m-%d %H:%M:%S", &time);

    std::tuple messageToDatabase
    {
        // std::pair{"ChannelID", channelID},
        std::pair{"SenderID", message.getUserID()},
        std::pair{"SendTime", timeStr}, 
        std::pair{"Msg", message.getMessageText()}
    };

    Table("ChannelMsgs").Insert()->columns(messageToDatabase)->execute();
}
