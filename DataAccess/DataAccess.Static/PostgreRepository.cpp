#include "PostgreRepository.hpp"

#include <Network/Message.hpp>
#include <Network/Primitives.hpp>
#include <ctime>
#include <iostream>

#include "Utility/Exception.hpp"

using namespace DataAccess;

PostgreRepository::PostgreRepository()
{
    _postgre =
        PostgreAdapter::getPostgre("dbname=postgres user=postgres hostaddr=127.0.0.1 port=5432");
    // TODO Import parameters from argument parser
}

std::vector<std::string> PostgreRepository::getAllChannelsList()
{
    std::vector<std::string> result;
    if (_postgre->isConnected())
    {
        auto channelListRow = _postgre->query("SELECT channel_name FROM channels;");

        for (auto item : channelListRow.value())
        {
            std::cout << item.at(0).c_str() << '\n';
            result.emplace_back(std::string(item.at(0).c_str()));
        }
    }
    return result;
}

std::vector<std::string> PostgreRepository::getMessageHistoryForUser(std::string userId)
{
    std::vector<std::string> result;
    if (_postgre->isConnected())
    {
        auto messageHistory =
            _postgre->query("SELECT message FROM messages WHERE user_id = " + userId);

        for (auto message : messageHistory.value())
        {
            std::cout << message.at(0).c_str() << '\n';
            result.emplace_back(std::string(message.at(0).c_str()));
        }
    }
    return result;
}

void PostgreRepository::storeMessage(Network::Message message)
{
    std::time_t t = std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp);

    Network::MessageInfo messageInfo;
    message >> messageInfo;

    if (_postgre->isConnected())
    {
        _postgre->query("INSERT INTO messages (user_id, message, timestamp) VALUES(" +
                        std::to_string(messageInfo.userID) + ",'" + messageInfo.message + "','" +
                        std::ctime(&t) + "')");
    }
}
