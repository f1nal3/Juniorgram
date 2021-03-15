#pragma once

#include <mutex>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

namespace DBPostgre
{

    // Thread safe singleton.
    class PostgreSQL
    {
    private:

        static std::mutex sm_static_mutex;
        static std::shared_ptr<PostgreSQL> spm_instance;

        std::mutex m_query_mutex;
        pqxx::connection m_connection;

    protected:

        PostgreSQL(const std::string_view& options)
            : m_connection{ pqxx::zview(options) } {}

    public:

        PostgreSQL(const PostgreSQL& other) = delete;
        PostgreSQL& operator = (const PostgreSQL& other) = delete;

        PostgreSQL(PostgreSQL&& other) = delete;
        PostgreSQL& operator = (PostgreSQL&& other) = delete;

    public:
        
        static std::shared_ptr<PostgreSQL> getPostgre(const std::string_view& options = "")
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

        pqxx::connection& getConnection()
        {
            return this->m_connection;
        }

        bool isConnected() const
        {
            return this->m_connection.is_open();
        }

        std::optional<pqxx::result> query(const std::string_view& query)
        {
            std::lock_guard<std::mutex> lock(this->m_query_mutex);

            pqxx::work work{ this->m_connection };

            auto res = work.exec(query);

            work.commit();

            if (!res.empty())
                return std::optional(res);

            return std::nullopt;
        }

    };

    std::mutex PostgreSQL::sm_static_mutex{};
    std::shared_ptr<PostgreSQL> PostgreSQL::spm_instance{};
    
}
