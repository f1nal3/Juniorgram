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

std::vector<std::string> FileRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    auto rows = database->select("channel_msgs", [channelID](const nlohmann::ordered_json& row) {
        return row.at("channel_id") == channelID ? true : false;
    });

    std::vector<std::string> messages;

    for (const auto& row : rows)
    {
        messages.emplace_back(row.at("msg"));
    }

    return messages;
}
}  // namespace DataAccess
