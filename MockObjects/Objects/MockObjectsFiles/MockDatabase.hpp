#pragma once

#include <DataAccess/IAdapter.hpp>
#include <Utility/Exception.hpp>

#include <memory>
#include <mutex>
#include <pqxx/pqxx>

#include <DataAccess.Postgre/DataBaseOptions.hpp>

namespace MockDatabase
{
using DataAccess::IAdapter;

/** 
* @class MockDatabase.
* This is a thread safe singleton.
*/
class MockDatabase final : public IAdapter
{
private:
    inline static std::mutex                          _staticMutex{};
    inline static std::shared_ptr<MockDatabase>       _instance{};
    static constexpr std::string_view                 _defaultOptions = DBOptions::real;

    std::mutex                        _queryMutex;
    std::unique_ptr<pqxx::connection> _connection;

public:
    /** 
    * @brief Method that creates new instance of Adapter.
    * It needs for technical purposes. Don't use it
    * (it's because I designed the interface badly).
    * Instead use getInstance method.
    * @param options - Connection options.
    * @return Pointer to current instanse of adapter.
    */
    static std::shared_ptr<MockDatabase> Instance(const std::string_view& options = {});

    explicit MockDatabase(const std::string_view& options) :
        _connection{std::make_unique<pqxx::connection>(pqxx::zview(options))} {}

    /**
    * @brief Method for executing SQL quries.
    * @details You shouldn't use this method because it's
    * low level accessing the database. Use it if you
    * want something specific from database, instead use PostgreQuery.
    * If you want to insert some strings, big text
    * or a timestamp you must wrap the string/text by single quotes.
    * You don't have to put ';' at the end of query.
    * @code
    * ...->query("SELECT * FROM table_name WHERE name = 'memorisecodead'");
    * @endcode
    * @param SQL query in the form of string.
    * @return Optional result.
    */
    std::optional<std::any> query(const std::string_view& query) override;

    /** 
    * @brief Method for getting connection object from pqxx.
    * @details This object using for low level accessing the
    * database. You probably won't need it.
    * @return pqxx::Connection object.
    */
    pqxx::connection& getConnection();

    /* 
    * @brief Method for checking the connection to the db.
    * @details Inside, it getConnection().is_open().
    * @return True - if connected. False - if not connected.
    */
    bool isConnected() const override;

    /** 
    * @brief Method for closing connection.
    * @details Method for change connection to the database.
    * @warning Be careful to use it because if you have many instances
    * of this class and after call this method, you may have
    * 'dangling' pointers.
    */
    void closeConnection() override;
};
}  // namespace MockDatabase
