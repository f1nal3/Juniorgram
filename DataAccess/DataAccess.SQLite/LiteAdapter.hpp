#pragma once
#include <memory>
#include <mutex>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <DataAccess/IAdapter.hpp>
#include <Utility/Exception.hpp>


namespace DataAccess
{
/**
 * @class LiteAdapter
 * @brief Adapter class for working with SQLite.
 * This is a thread safe singleton.
 */
class LiteAdapter : public IAdapter
{
public:
    Q_DISABLE_COPY_MOVE(LiteAdapter)

    /// Default destructor
    ~LiteAdapter() override = default;

    /**
     * @brief Method that gets us single Lite adapter instance.
     * @details It needs for technical purposes. Don't use it /
     *  (it's because I designed the interface badly). /
     *  Instead use getInstance method. /
     * @param db_name - Data Base name.
     * @return Pointer to current instance of Lite adapter.
     */
    static std::shared_ptr<LiteAdapter> Instance(const std::string_view& db_name);

    /// Create database adapter with a db name
    explicit LiteAdapter(const std::string_view& db_name);

public:
    /**
     * @brief Method for executing SQL quries.
     * @details You shouldn't use this method because it's
     *      low level accessing the database. Use it if you
     *      want something specific from database, instead use LiteTable.
     *      If you want to insert some strings, big text
     *      or a timestamp you must wrap the string/text by single quotes.
     *      You don't have to put ';' at the end of query.
     * @code
     *      ...->query("SELECT * FROM table_name WHERE name = 'Alex'");
     * @endcode
     * @param SQL query in the form of string.
     * @return Optional result.
     */
    std::optional<std::any> query(const std::string_view& query) override;

    /**
     * @brief Method for checking the connection to the db.
     * @details Inside, it getConnection().is_open().
     * @return True - if connected. False - if not connected.
     */
    bool isConnected() const override;

    /**
     * @brief Method for closing connection.
     * @details Method for change connection to the database.
     */
    void closeConnection() override;

private:
    inline static std::mutex                   mtx{};
    inline static std::shared_ptr<LiteAdapter> sqlite_instance{};

    inline static constexpr std::string_view m_db_name = "SQLiteDB.db";

    std::unique_ptr<QSqlDatabase> db_connection;
    std::mutex                    query_mtx;
};
}  /// namespace DataAccess
