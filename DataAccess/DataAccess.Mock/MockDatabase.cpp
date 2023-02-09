#include "MockDatabase.hpp"

namespace MockObject
{
std::shared_ptr<MockDatabase> MockDatabase::Instance([[maybe_unused]] const std::string_view& options)
{
    std::scoped_lock<std::mutex> lock(_staticMutex);

    if (_instance == nullptr)
    {
        _instance = std::shared_ptr<MockDatabase>
            (std::make_shared<MockDatabase>(TestUtility::testProperties));
    }
    else
    {
        _instance = std::shared_ptr<MockDatabase>
            (std::make_shared<MockDatabase>(options));
    }

    return _instance;
}

std::optional<std::any> MockDatabase::query(const std::string_view& query)
{
    if (query == "channels")
    {
        return _connection->getChannelsRepoData().getStorageData();
    }

    if (query == "replies")
    {
        return _connection->getRepliesRepoData().getStorageData();
    }

    if (query == "direct_msgs")
    {
        return _connection->getDirectMessageRepoData().getStorageData();
    }

    if (query == "msgs")
    {
        return _connection->getMessageRepoData().getStorageData();
    }

    if (query == "user_registration")
    {
        return _connection->getRegisterRepoData().getStorageData();
    }

    if (query == "user_login")
    {
        return _connection->getLoginRepoData().getStorageData();
    }

    return std::vector<std::string>();
}

bool MockDatabase::isConnected() const 
{
    return _connection.get();
}

void MockDatabase::closeConnection() 
{
    return _connection.reset();
}
}  /// namespace MockObject
