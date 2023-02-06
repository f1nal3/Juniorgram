#include "MockDatabase.hpp"

namespace MockDatabase
{
std::shared_ptr<MockDatabase> MockDatabase::Instance([[maybe_unused]] const std::string_view& options)
{
    std::scoped_lock<std::mutex> lock(_staticMutex);

    if (_instance == nullptr)
    {
        _instance = std::shared_ptr<MockDatabase>
            (std::make_shared<MockDatabase>(TestUtility::testProperties));
    }

    return _instance;
}

std::optional<std::any> MockDatabase::query(const std::string_view& query)
{
    return _connection->getStorageData().emplace_back(query);
}

bool MockDatabase::isConnected() const 
{
    return _connection.get();
}

void MockDatabase::closeConnection() 
{
    return _connection.reset();
}
}  // namespace MockDatabase
