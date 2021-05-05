#pragma once

#include <mutex>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

namespace DataAccess
{
/** @class PostgreAdapter
*   @brief Adapter class for working with PostgreSQL. \
*   The is thread safe singleton.
*/
class PostgreAdapter
{
private:
    inline static std::mutex ms_static_mutex{};
    inline static std::shared_ptr<PostgreAdapter> msp_instance{};
    inline static std::string_view ms_newOptions{};
    inline static constexpr std::string_view ms_defaultOptions =
        "dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432";

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
    /** @brief Method for getting PostgreAdapter instances.
    *   @details This method return the instance with standart options \
    *    (dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432).
    *   @return PostgreAdapter instance: std::shared_ptr<...>
    */
    static std::shared_ptr<PostgreAdapter> getPostgre();
    /** @brief Method for getting PostgreAdapter instances.
     *  @details This method return the instance with your options. \
     *   Like: dbname=... user=... hostaddr=... port=... ... . \
     *   There are another options like: ssh, password, etc. \
     *   This options must be passed as a single string. \
     *  @param Your options.
     *  @return PostgreAdapter instance: std::shared_ptr<...>
     */
    static std::shared_ptr<PostgreAdapter> getPostgre(const std::string_view& options);

    /** @brief Method for executing SQL quries.
    *   @details You shouldn't use this method because it's \
    *    low level accessing the database. Use it if you \
    *    want something specific from database, instead use \
    *    PostgreAbstractionLayout Table. \
    *    If you want to insert some strings, big text \
    *    or a timestamp you must wrap the string/text by single quotes. \
    *    You don't have to put ';' at the end of query.
    *   @code 
    *   ...->query("SELECT * FROM table_name WHERE name = 'Alex'");
    *   @endcode
    *   @param SQL query in the form of string.
    *   @return Optional result.
    */
    std::optional<pqxx::result> query(const std::string_view& query);

    /** @brief Method for getting connection object from pqxx.
    *   @details This object using for low level accessing the \
    *    database. You probably won't need it.
    *   @return pqxx::Connection object.
    */
    pqxx::connection& getConnection(void);

    /* @brief Method for checking the connection to the db.
    *  @details Inside, it getConnection().is_open().
    *  @return True - if connected. False - if not connected.
    */
    bool isConnected(void) const;

    /** @brief Method for closing connection.
    *   @details You should use it if you know for sure that \
    *    you don't need access to the database.
    */
    void closeConnection(void);
};

} // namespace DataAccess
