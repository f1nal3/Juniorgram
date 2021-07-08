#pragma once
#include "Utility/WarningSuppression.hpp"

#include <iostream>
#include <memory>
#include <mutex>
#include <sqlcipher/sqlite3.h>
#include <DataAccess/IAdapter.hpp>
#include <Utility.Static/Cryptography.hpp>
#include <Utility/Exception.hpp>
#include <Utility.Static/MACAddress.hpp>


namespace DataAccess
{
/** @class QSQLCipherAdapter
 *   @brief Adapter class for working with PostgreSQL. \
 *   The is thread safe singleton.
 */

struct sqlite3_deleter
{
    void operator()(sqlite3* sql);
};

using sqlite3_ptr = std::unique_ptr<sqlite3, sqlite3_deleter>;

sqlite3_ptr make_sqlite3(const std::string_view& dbName);

class SQLCipherAdapter final : public IAdapter
{
public:
    /** @brief Method that creates new instance of Adapter. \
     *    It needs for technical purposes. Don't use it \
     *    (it's because I designed the interface badly). \
     *    Instead use getInstance method.
     *   @params options - Connection options.
     *   @return Pointer to current instanse of Postgre adapter.
     */
    SQLCipherAdapter(const SQLCipherAdapter& other) = delete;
    SQLCipherAdapter& operator=(const SQLCipherAdapter& other) = delete;

    SQLCipherAdapter(SQLCipherAdapter&& other) = delete;
    SQLCipherAdapter& operator=(SQLCipherAdapter&& other) = delete;
   
    ~SQLCipherAdapter();
    static std::shared_ptr<SQLCipherAdapter> Instance(const std::string_view& options = {});
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

    bool isConnected(void) const override;

    /** @brief Method for closing connection.
     *   @details Method for change connection to the database.
     *   @warning Be careful to use it because if you have many instances \
     *    of this class and after call this method, you may have \
     *    'dangling' pointers.
     */
    void closeConnection(void) override;

protected:
    SQLCipherAdapter(const std::string_view& dbName) 
        : mDB(make_sqlite3(dbName))
    {
    }

private:
    inline static std::mutex mStaticMutex{};
    inline static std::shared_ptr<SQLCipherAdapter> mspInstance{};
    inline static constexpr std::string_view msDefaultDBName = "refr.db";

    std::mutex mQueryMutex{};
    sqlite3_ptr mDB{nullptr};
    std::string mErrMsg{};
};

}  // namespace DataAccess
