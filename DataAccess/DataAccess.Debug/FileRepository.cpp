#include "FileRepository.hpp"
#include "Utility\Exception.hpp"
#include "Utility\Utility.hpp"

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

std::vector<std::string> FileRepository::getMessageHistoryForUser(const std::string& userID)
{
    auto rows = database->select("messages", [userID](const nlohmann::ordered_json& row) {
        return row.at("user_id") == std::stoll(userID) ? true : false;
    });

    std::vector<std::string> messages;

    for (const auto& row : rows)
    {
        messages.emplace_back(row.at("message"));
    }

    return messages;
}

void FileRepository::storeMessage(const Network::UserMessage& message)
{
    std::string formattedTime = std::string{30, '\0'};

    std::time_t timestamp = std::chrono::system_clock::to_time_t(message.getTimestamp());
    std::tm localTime     = Utility::safe_localtime(timestamp);
    std::strftime(formattedTime.data(), formattedTime.size(), "%Y-%m-%d %H:%M:%S", &localTime);

    database->insert("messages",
                     {std::to_string(message.getUserID()), message.getMessageText(), formattedTime},
                     {"user_id", "message", "timestamp"});
}
}  // namespace DataAccess
