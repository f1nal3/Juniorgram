#include "PostgreAdapter.hpp"

namespace DataAccess
{
    std::shared_ptr<PostgreAdapter> PostgreAdapter::Instance(const std::string_view& options)
    {
        std::scoped_lock<std::mutex> lock(_staticMutex);

        if (_instance == nullptr)
        {
            _instance = std::shared_ptr<PostgreAdapter>(new PostgreAdapter(options));
        }

        return _instance;
    }

    pqxx::connection& PostgreAdapter::getConnection() 
    {
        return *_connection; 
    }

    bool PostgreAdapter::isConnected() const 
    {
        return _connection->is_open(); 
    }

    std::optional<std::any> PostgreAdapter::query(const std::string_view& query)
    {
        std::scoped_lock<std::mutex> lock(_queryMutex);

        if (this->isConnected())
        {
            pqxx::work work{ *_connection };

            auto res = work.exec(query);
            work.commit();

            if (!res.empty())
            {
                return std::optional{ std::any{ res } };
            }
        }
        else
        {
            throw Utility::OperationDBException("Connection with database was released!", __FILE__, __LINE__);
        }

        return std::nullopt;
    }

    void PostgreAdapter::closeConnection() 
    { 
        _instance.reset();
        _connection.reset();
    }

} // namespace DataAccess
