#include "PostgreRepository.hpp"

using namespace DataAccess;

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;

    auto channelListRow = Table("channels").Select()->columns({"channel_name"})->execute();

    if (channelListRow.has_value())
    {
        for (auto item : channelListRow.value())
        {
            std::cout << item.at(0).c_str() << '\n';
            result.emplace_back(std::string(item.at(0).c_str()));
        }
    }

    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    std::vector<std::string> result;

    auto messageHistoryRow = 
        Table("channel_msgs").Select()->columns({"msg"})->Where("channel_id = " + channelID)->execute();


    if (messageHistoryRow.has_value())
    {
        for (auto message : messageHistoryRow.value())
        {
            std::cout << message.at(0).c_str() << '\n';
            result.emplace_back(std::string(message.at(0).c_str()));
        }
    }

    return result;
}

void PostgreRepository::storeMessage(const Network::MessageInfo& message, const std::uint64_t channelID)
{
    char timeStampStr[35];

    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time  = Utility::safe_localtime(t);
    std::strftime(timeStampStr, 35, "%Y-%m-%d %H:%M:%S.0+00", &time);

    std::tuple messageToDatabase
    {
        std::pair{"channel_id", channelID},
        std::pair{"sender_id", message.userID},
        std::pair{"send_time", timeStampStr}, 
        std::pair{"msg", message.message}
    };
    Table("channel_msgs").Insert()->columns(messageToDatabase)->execute();
}
