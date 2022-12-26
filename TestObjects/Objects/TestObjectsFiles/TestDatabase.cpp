#include "TestDatabase.hpp"

namespace TestDatabase
{
std::shared_ptr<TestDatabase> TestDatabase::Instance(const std::string_view& options)
{
    std::scoped_lock<std::mutex> lock(_staticMutex);

    if (_instance == nullptr)
    {
        _instance = std::shared_ptr<TestDatabase>(std::make_shared<TestDatabase>(options.empty() ? _defaultOptions : options));
    }

    return _instance;
}

std::optional<std::any> TestDatabase::query(const std::string_view& query)
{
    std::scoped_lock<std::mutex> lock(_queryMutex);

    if (this->isConnected())
    {
        pqxx::work work{*_connection};

        auto res = work.exec(query);
        work.commit();

        if (!res.empty())
        {
            return std::optional{std::any{res}};
        }
    }
    else
    {
        throw Utility::OperationDBException
        (
            "Connection with database was released!",
            __FILE__, __LINE__
        );
    }

    return std::nullopt;
}

void TestDatabase::closeConnection()
{
    _instance.reset();
    _connection.reset();
}

pqxx::connection& TestDatabase::getConnection() 
{
    return *_connection; 
}

bool TestDatabase::isConnected() const 
{
    return _connection->is_open(); 
}
}  // namespace TestDatabase
