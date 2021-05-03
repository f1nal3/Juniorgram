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

std::vector<std::string> FileRepository::getMessageHistoryForUser(const unsigned channelID)
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

void FileRepository::storeMessage(const Network::MessageInfo& messageInfo, const unsigned channelID)
{
    std::string formattedTime = std::string{30, '\0'};

    std::time_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime = Utility::safe_localtime(timestamp);
    std::strftime(formattedTime.data(), formattedTime.size(), "%Y-%m-%d %H:%M:%S", &localTime);

    database->insert("messages",
                     {std::to_string(channelID), std::to_string(messageInfo.userID), formattedTime, messageInfo.message},
                     {"channel_id", "sender_id", "send_time", "msg"});
}
}  // namespace DataAccess
