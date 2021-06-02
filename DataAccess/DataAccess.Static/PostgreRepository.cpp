#include "PostgreRepository.hpp"
#include "RepositoryUnits.hpp"

using namespace DataAccess;

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;

    auto channelListRow = std::get<0>(PTable("channels").Select()->columns({"channel_name"})->execute());

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

    auto messageHistoryRow = std::get<0>(PTable("channel_msgs")
                                         .Select()
                                         ->columns({"msg"})
                                         ->Where("channel_id = " + std::to_string(channelID))
                                         ->execute());

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
    std::string timeStampStr = nowTimeStampStr();

    std::tuple dataForMsgs
    {
        std::pair{"sender_id", message.userID},
        std::pair{"send_time", timeStampStr},
        std::pair{"msg", message.message }
    };
    PTable("msgs").Insert()->columns(dataForMsgs)->execute();
    
    // ID will not be autoincremented in the future. Later we are going to use postgre
    // alghorithms to create it.
    std::uint64_t msgID = std::get<0>(PTable("msgs")
                                      .Select()
                                      ->columns({"max(msg_id)"})
                                      ->execute())
                                      .value()[0][0].as<std::uint64_t>();
    std::tuple dataForChannelMsgs
    {
        std::pair{"channel_id", channelID},
        std::pair{"msg_id", msgID}
    };
    
    PTable("channel_msgs").Insert()->columns(dataForChannelMsgs)->execute();
}
