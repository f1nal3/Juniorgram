#pragma once

#include <mutex>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

namespace DataAccess
{
// Thread safe singleton.
class PostgreAdapter
{
private:
    inline static std::mutex ms_static_mutex{};
    inline static std::shared_ptr<PostgreAdapter> msp_instance{};

    std::mutex m_query_mutex;
    pqxx::connection m_connection;

protected:
    PostgreAdapter(const std::string_view& options) : m_connection{pqxx::zview(options)} {}

public:
    PostgreAdapter(const PostgreAdapter& other) = delete;
    PostgreAdapter& operator=(const PostgreAdapter& other) = delete;

    PostgreAdapter(PostgreAdapter&& other) = delete;
    PostgreAdapter& operator=(PostgreAdapter&& other) = delete;

public:
    static std::shared_ptr<PostgreAdapter> getPostgre(const std::string_view& options = "");

    std::optional<pqxx::result> query(const std::string_view& query);

    pqxx::connection& getConnection(void);

    bool isConnected(void) const;

    void closeConnection(void);
};

} // namespace DataAccess
