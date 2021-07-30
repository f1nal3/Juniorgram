#include "FileRepository.hpp"
#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

namespace DataAccess
{
FileRepository::FileRepository() 
{ 
    database = std::make_unique<FileDB>("Debug"); 
}

std::vector<Network::ChannelInfo> FileRepository::getAllChannelsList()
{
    auto rows = database->select("channels");
    std::vector<Network::ChannelInfo> channels;

    for (const auto& row : rows)
    {
        Network::ChannelInfo channelInfo(row.at("creator_id"), row.at("id"), row.at("channel_name"));
        channels.emplace_back(channelInfo);
    }

    return channels;
}

std::vector<Network::MessageStoringInfo> FileRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    auto rows = database->select("channel_msgs", [channelID](const nlohmann::ordered_json& row) {
        return row.at("channel_id") == channelID ? true : false;
    });

    std::vector<Network::MessageStoringInfo> messages;

    Network::MessageStoringInfo mi;
    mi.channelID = channelID;
    for (const auto& row : rows)
    {
        mi.message = row.at("msg");
        mi.time    = row.at("send_time");
        mi.userID  = row.at("sender_id");
        messages.emplace_back(row);
    }

    return messages;
}
}  // namespace DataAccess
