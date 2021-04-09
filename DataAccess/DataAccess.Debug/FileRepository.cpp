#include "FileRepository.hpp"
#include "Utility\Exception.hpp"

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
            channels.emplace_back(row.at("name"));
	}

	return channels;
}

std::vector<std::string> FileRepository::getMessageHistoryForUser(std::string userID) 
{
    auto rows = database->select("channels",
        [userID](const nlohmann::ordered_json& row){ return row.at("user_id") == userID ? true : false; });

    std::vector<std::string> messages;

    for (const auto& row : rows)
    {
        messages.emplace_back(row.at("message"));
    }

    return messages;
}

void FileRepository::storeMessages(std::vector<std::string> messageList)
{
    throw Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
}

}