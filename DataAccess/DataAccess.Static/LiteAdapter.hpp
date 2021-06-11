#pragma once

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <DataAccess/IAdapter.hpp>
#include <Utility/Exception.hpp>

#include <memory>
#include <mutex>

namespace  DataAccess
{
class LiteAdapter : public IAdapter
{
public:
    inline static std::mutex mtx{};
    inline static std::shared_ptr<LiteAdapter> sqlite_instance{};
    inline static constexpr std::string_view path = "Client/SQLiteDB.db";
    static std::shared_ptr<LiteAdapter> Instance(const std::string_view& path);
private:
    std::unique_ptr<QSqlDatabase> db_connection;
    std::mutex query_mtx;
protected:
    LiteAdapter(const std::string_view& path);
public:
    LiteAdapter(const LiteAdapter& other) = delete;
    LiteAdapter& operator=(const LiteAdapter& other) = delete;

    LiteAdapter(LiteAdapter&& other) = delete;
    LiteAdapter& operator=(LiteAdapter&& other) = delete;
public:
    std::optional<std::any> query(const std::string_view& query) override;
    bool isConnected(void) const override;
    void closeConnection(void) override;
};
}
