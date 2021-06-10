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
class SQLiteAdapter : public IAdapter
{
public:
    inline static std::mutex mtx{};
    inline static std::shared_ptr<SQLiteAdapter> sqlite_instance{};
    inline static const QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    static std::shared_ptr<SQLiteAdapter> Instance(const QSqlDatabase& database);
private:
    std::unique_ptr<QSqlDatabase> db_connection;
protected:
    SQLiteAdapter(const QSqlDatabase& database)
        : db_connection{std::make_unique<QSqlDatabase>(database)} {}
public:
    SQLiteAdapter(const SQLiteAdapter& other) = delete;
    SQLiteAdapter& operator=(const SQLiteAdapter& other) = delete;

    SQLiteAdapter(SQLiteAdapter&& other) = delete;
    SQLiteAdapter& operator=(SQLiteAdapter&& other) = delete;
public:
    std::optional<std::any> query(const std::string_view& query) override;
    bool isConnected(void) const override;
    void closeConnection(void) override;
};
}
