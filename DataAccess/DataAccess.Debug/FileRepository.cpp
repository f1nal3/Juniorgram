#include "FileRepository.hpp"

#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

namespace DataAccess
{
FileRepository::FileRepository() 
{ 
    _database = std::make_unique<FileDB>("Debug"); 
}

std::vector<Models::ChannelInfo> FileRepository::getAllChannelsList()
{
    auto rows = _database->select("channels");
    std::vector<Models::ChannelInfo> channels;

    Models::ChannelInfo channelInfo;
    for (const auto& row : rows)
    {
        channelInfo.creatorID = row.at("creator_id");
        channelInfo.channelName = row.at("channel_name");
        channelInfo.channelID   = row.at("id");
        channels.emplace_back(channelInfo);
    }

    return channels;
}

std::vector<Models::MessageInfo> FileRepository::getMessageHistoryForUser(const std::uint64_t channelID)
{
    auto rows = _database->select("channel_msgs", [channelID](const nlohmann::ordered_json& row) {
        return row.at("channel_id") == channelID ? true : false;
    });

    std::vector<Models::MessageInfo> messages;

    Models::MessageInfo mi;
    mi.channelID = channelID;
    for (auto&& row : rows)
    {
        mi.message   = row.at("msg");
        mi.time      = row.at("send_time");
        mi.senderID  = row.at("sender_id");
        mi.userLogin  = row.at("username");
        messages.emplace_back(mi);
    }

    return messages;
}
}  /// namespace DataAccess
