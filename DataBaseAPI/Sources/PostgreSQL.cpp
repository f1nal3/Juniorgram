#include "../Headers/DataBasePostgreSQL.h"

namespace DBPostgre
{
	std::shared_ptr<PostgreSQL> PostgreSQL::getPostgre(const std::string_view& options = "") 
	{
        std::lock_guard<std::mutex> lock(sm_static_mutex);

        if (spm_instance == nullptr)
        {
            if (options.empty())
            {
                throw pqxx::broken_connection(
                    "Instance didn't create because options are incorrect!");
            }

            spm_instance = std::shared_ptr<PostgreSQL>(new PostgreSQL(options));
        }

        return spm_instance;
    }

    pqxx::connection& PostgreSQL::getConnection() 
    { 
        return this->m_connection; 
    }

    bool PostgreSQL::isConnected() const
    { 
        return this->m_connection.is_open(); 
    }

    std::optional<pqxx::result> PostgreSQL::quary(const std::string_view& quary)
    {
        std::lock_guard<std::mutex> lock(this->m_quary_mutex);

        pqxx::work work{ this->m_connection };

        auto res = work.exec(quary);

        work.commit();

        if (!res.empty())
            return std::optional(res);

        return std::nullopt;
    }
}
