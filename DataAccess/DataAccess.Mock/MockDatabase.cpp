#include "MockDatabase.hpp"

namespace MockObject
{
std::shared_ptr<MockDatabase> MockDatabase::Instance(const std::string_view& options)
{
    if (_instance == nullptr)
    {
        _instance = std::make_shared<MockDatabase>(options);
    }
    return _instance;
}

std::optional<std::any> MockDatabase::query(const std::string_view& query)
{
    return std::vector<std::string>().emplace_back(query);
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
