#include "PostgreAdapter.hpp"

namespace DataAccess
{

    std::shared_ptr<PostgreAdapter> PostgreAdapter::getPostgre() 
    {
        return getPostgre(ms_standartOptions);
    }
	std::shared_ptr<PostgreAdapter> PostgreAdapter::getPostgre(const std::string_view& options)
    {
        std::scoped_lock<std::mutex> lock(ms_static_mutex);

        if (msp_instance == nullptr)
        {
            if (options.empty())
            {  
                throw pqxx::broken_connection(
                    "Instance didn't create because options are incorrect!");
            }

            ms_options   = options;
            msp_instance = std::shared_ptr<PostgreAdapter>(new PostgreAdapter(options));
        }

        return msp_instance;
    }

    pqxx::connection& PostgreAdapter::getConnection(void) 
    {
        return m_connection; 
    }

    bool PostgreAdapter::isConnected(void) const 
    {
        return m_connection.is_open(); 
    }

    std::optional<pqxx::result> PostgreAdapter::query(const std::string_view& query)
    {
        std::scoped_lock<std::mutex> lock(m_query_mutex);

        pqxx::work work{ m_connection };

        auto res = work.exec(query);

        work.commit();

        if (!res.empty()) 
            return std::optional(res);

        return std::nullopt;
    }

    void PostgreAdapter::closeConnection(void) 
    { 
        msp_instance.reset();
    }

}
