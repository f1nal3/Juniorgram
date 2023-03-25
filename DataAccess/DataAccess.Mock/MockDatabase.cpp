#include "MockDatabase.hpp"

namespace MockObject
{
std::shared_ptr<MockDatabase> MockDatabase::Instance(const std::string_view& options)
{
    std::scoped_lock<std::mutex> lock(_staticMutex);

    if (_instance == nullptr)
    {
        _instance = std::shared_ptr<MockDatabase>(new MockDatabase(std::move(options)));
    }

    return _instance;
}

[[maybe_unused]] std::optional<std::any> MockDatabase::query(const std::string_view& query) 
{
    return std::optional<std::any>().emplace(query);
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
