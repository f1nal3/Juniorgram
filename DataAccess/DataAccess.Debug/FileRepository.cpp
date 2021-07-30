#include "FileRepository.hpp"
#include "Utility/Exception.hpp"
#include "Utility/Utility.hpp"

namespace DataAccess
{
FileRepository::FileRepository() 
{ 
    database = std::make_unique<FileDB>("Debug"); 
}

std::vector<std::string> FileRepository::getAllChannelsList()
{
    auto rows = database->select("channels");
    std::vector<std::string> channels;

    for (const auto& row : rows)
    {
        channels.emplace_back(row.at("channel_name"));
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
