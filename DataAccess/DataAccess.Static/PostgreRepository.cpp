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

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(const unsigned channelID)
{
    std::vector<std::string> result;

    auto messageHistoryRow = 
        Table("channel_msgs").Select()->columns({"msg"})->where("channel_id = " + channelID)->execute();


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

void PostgreRepository::storeMessage(const Network::MessageInfo& message, const unsigned channelID)
{
    std::string timeStr = PostgreAdapter::getPostgre()
                  ->query("select now()")
                  .value().at(0).at(0)
                  .as<std::string>();

    std::tuple messageToDatabase
    {
        std::pair{"channel_id", channelID},
        std::pair{"sender_id", message.userID},
        std::pair{"send_time", timeStr}, 
        std::pair{"msg", message.message}
    };
    Table("channel_msgs").Insert()->columns(messageToDatabase)->execute();
}
