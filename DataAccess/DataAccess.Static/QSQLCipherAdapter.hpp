#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTemporaryDir>
#pragma once

#include <DataAccess/IAdapter.hpp>
#include <Utility/Exception.hpp>
#include <memory>
#include <mutex>
#include <pqxx/pqxx>

namespace DataAccess
{
/** @class QSQLCipherAdapter
 *   @brief Adapter class for working with PostgreSQL. \
 *   The is thread safe singleton.
 */
class QSQLCipherAdapter final : public IAdapter
{
public:
    QSQLCipherAdapter(const QSQLCipherAdapter& other) = delete;
    QSQLCipherAdapter& operator=(const QSQLCipherAdapter& other) = delete;

    QSQLCipherAdapter(QSQLCipherAdapter&& other) = delete;
    QSQLCipherAdapter& operator=(QSQLCipherAdapter&& other) = delete;
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
    std::optional<std::any> query(const std::string_view& query) override;

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
    bool isConnected(void) const override;

    /** @brief Method for closing connection.
     *   @details Method for change connection to the database.
     *   @warning Be careful to use it because if you have many instances \
     *    of this class and after call this method, you may have \
     *    'dangling' pointers.
     */
    void closeConnection(void) override;

public:
    /** @brief Method that creates new instance of Adapter. \
     *    It needs for technical purposes. Don't use it \
     *    (it's because I designed the interface badly). \
     *    Instead use getInstance method.
     *   @params options - Connection options.
     *   @return Pointer to current instanse of Postgre adapter.
     */
    static std::shared_ptr<QSQLCipherAdapter> Instance(const std::string_view& options = {});

protected:
    QSQLCipherAdapter(const std::string_view& options)
        : m_connection{std::make_unique<pqxx::connection>(pqxx::zview(options))}
    {
    }

private:
    inline static std::mutex ms_static_mutex{};
    inline static std::shared_ptr<QSQLCipherAdapter> msp_instance{};
    inline static constexpr std::string_view ms_defaultOptions =
        "dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432";

    std::mutex m_query_mutex;
    std::unique_ptr<pqxx::connection> m_connection;
};

}  // namespace DataAccess
