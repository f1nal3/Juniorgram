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

    Network::ChannelInfo channelInfo;
    for (const auto& row : rows)
    {
        channelInfo.creatorID = row.at("creator_id");
        channelInfo.channelName = row.at("channel_name");
        channelInfo.channelID   = row.at("id");
        channels.emplace_back(channelInfo);
    }

    return channels;
}

std::vector<Network::MessageInfo> FileRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    auto rows = database->select("channel_msgs", [channelID](const nlohmann::ordered_json& row) {
        return row.at("channel_id") == channelID ? true : false;
    });

    std::vector<Network::MessageInfo> messages;

    Network::MessageInfo mi;
    mi.channelID = channelID;
    for (auto&& row : rows)
    {
        mi.message   = row.at("msg");
        mi.time      = row.at("send_time");
        mi.senderID  = row.at("sender_id");
        messages.emplace_back(mi);
    }

    return messages;
}
}  // namespace DataAccess
