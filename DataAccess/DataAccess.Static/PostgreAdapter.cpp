#include "PostgreAdapter.hpp"

namespace DataAccess
{
    std::shared_ptr<PostgreAdapter> PostgreAdapter::Instance(const std::string_view& options)
    {
        std::scoped_lock<std::mutex> lock(ms_static_mutex);

        if (msp_instance == nullptr)
        {
            msp_instance = std::shared_ptr<PostgreAdapter>(new PostgreAdapter(options.empty() ? ms_defaultOptions : options));
        }

        return msp_instance;
    }

    pqxx::connection& PostgreAdapter::getConnection(void) 
    {
        return *m_connection; 
    }

    bool PostgreAdapter::isConnected(void) const 
    {
        return m_connection->is_open(); 
    }

    std::optional<std::any> PostgreAdapter::query(const std::string_view& query)
    {
        std::scoped_lock<std::mutex> lock(m_query_mutex);

        if (this->isConnected())
        {
            pqxx::work work{ *m_connection };

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

    void PostgreAdapter::closeConnection(void) 
    { 
        msp_instance.reset();
        m_connection.reset();
    }

}
