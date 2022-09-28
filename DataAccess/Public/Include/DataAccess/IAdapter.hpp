#pragma once

#include <any>
#include <memory>
#include <optional>
#include <string_view>

namespace DataAccess
{
    /**
    * @class IAdapter
    * @brief An abstract class manages the connection to database
    * @details Class allows connect to database and disconnect from it,
    * can return options, which are used for connecting to database.
    * Every database have its own adapter, which is implementation of IAdapter.
    */
class IAdapter
{
public:
    /**
    * @brief Method for getting Adapter instances.
    * @details This method return the instance with your options. \n
    *	 Like: dbname=... user=... hostaddr=... port=... ... . \n
    *	 There are another options like: ssh, password, etc.
    *	 This options must be passed as a single string.
    *	 By default:
    *     (dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432).
    * @param Params for connecting to the database.
    * @return Adapter instance: std::shared_ptr<...>
    */
    template <typename T>
    static std::shared_ptr<T> getInstance(const std::string_view& options = {}) { return T::Instance(options); }

    /**
    * @brief Method for executing SQL queries.
    * @details This method use raw SQL query, so it isn`t recommended to use this function,
    * because every type of database has own interface for creating queries.
    * Usage example you can see in PostgreAdapter.hpp.
    */
    virtual std::optional<std::any> query(const std::string_view& query) = 0;

    virtual bool isConnected(void) const = 0;

    virtual void closeConnection(void) = 0;

    virtual ~IAdapter() {}
};
}  // namespace DataAccess
