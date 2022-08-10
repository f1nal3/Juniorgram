#include "MockPostgreAdapter.hpp"

namespace MockRepository
{
std::shared_ptr<MockPostgreAdapter> MockPostgreAdapter::Instance(const std::string_view& options)
{
    std::scoped_lock<std::mutex> lock(_staticMutex);

    if (_instance == nullptr)
    {
        _instance = std::shared_ptr<MockPostgreAdapter>(new MockPostgreAdapter(options.empty() ? _defaultOptions : options));
    }

    return _instance;
}

std::optional<std::any> MockPostgreAdapter::query(const std::string_view& query)
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

void MockPostgreAdapter::closeConnection(void)
{
    _instance.reset();
    _connection.reset();
}

pqxx::connection& MockPostgreAdapter::getConnection(void) 
{
    return *_connection; 
}

bool MockPostgreAdapter::isConnected(void) const 
{
    return _connection->is_open(); 
}
}  // namespace MockRepository
