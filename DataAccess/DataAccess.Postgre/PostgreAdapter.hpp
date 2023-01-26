#pragma once

#include <mutex>
#include <memory>

#include <pqxx/pqxx>

#include <Utility/JGExceptions.hpp>
#include <DataAccess/IAdapter.hpp>

namespace DataAccess
{

/** @class PostgreAdapter
*   @brief Adapter class for working with PostgreSQL. 
*   This is a thread safe singleton.
*/
class PostgreAdapter final : public IAdapter
{
private:
    inline static std::mutex                        _staticMutex{};
    inline static std::shared_ptr<PostgreAdapter>   _instance{};

    std::mutex                                      _queryMutex;
    std::unique_ptr<pqxx::connection>               _connection;

protected:
    PostgreAdapter(const std::string_view& options) 
        : _connection{std::make_unique<pqxx::connection>(pqxx::zview(options))} {}

public:

    PostgreAdapter(const PostgreAdapter& other) = delete;
    PostgreAdapter& operator=(const PostgreAdapter& other) = delete;

    PostgreAdapter(PostgreAdapter&& other) = delete;
    PostgreAdapter& operator=(PostgreAdapter&& other) = delete;

    virtual ~PostgreAdapter() {}

    /** @brief Method that creates new instance of Adapter. 
    *    It needs for technical purposes. Don't use it 
    *    (it's because I designed the interface badly). 
    *    Instead use getInstance method.
    *   @param options - Connection options.
    *   @return Pointer to current instance of Postgre adapter.
    */
    static std::shared_ptr<PostgreAdapter> Instance(const std::string_view& options = {});

    /** @brief Method for executing SQL queries.
    *   @details You shouldn't use this method because it's 
    *    low level accessing the database. Use it if you 
    *    want something specific from database, instead use PostgreQuery. 
    *    If you want to insert some strings, big text 
    *    or a timestamp you must wrap the string/text by single quotes. 
    *    You don't have to put ';' at the end of query.
    *   @code 
    *   ...->query("SELECT * FROM table_name WHERE name = 'username'");
    *   @endcode
    *   @param SQL query in the form of string.
    *   @return Optional result.
    */
    std::optional<std::any> query(const std::string_view& query) override;

    /** @brief Method for getting connection object from pqxx.
    *   @details This object using for low level accessing the 
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
    *   @warning Be careful to use it because if you have many instances 
    *    of this class and after call this method, you may have 
    *    'dangling' pointers.
    */
    void closeConnection(void) override;
};
} // namespace DataAccess
