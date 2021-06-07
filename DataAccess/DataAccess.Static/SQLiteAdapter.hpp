#pragma once

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <DataAccess/IAdapter.hpp>

namespace  DataAccess
{
class SQLiteAdapter : public IAdapter
{
private:
    QSqlDatabase db;
protected:
    SQLiteAdapter();
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
