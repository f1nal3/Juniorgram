#include "SQLiteAdapter.hpp"

namespace DataAccess
{

std::shared_ptr<SQLiteAdapter> SQLiteAdapter::Instance(const QSqlDatabase &database)
{
    std::scoped_lock<std::mutex> lock(mtx);
    if(sqlite_instance == nullptr)
    {
        sqlite_instance = std::shared_ptr<SQLiteAdapter>(new SQLiteAdapter(database));
    }
    return sqlite_instance;
}

std::optional<std::any> SQLiteAdapter::query(const std::string_view &query)
{
    std::scoped_lock<std::mutex> lock(mtx);

    if(this->isConnected())
    {
        QSqlQuery work{ *db_connection };
        work.exec(QString("1%").arg(query.data()));


    }
    else
    {
        throw Utility::OperationDBException("Connection with database was released!", __FILE__, __LINE__);
    }

    return std::nullopt;
}

bool SQLiteAdapter::isConnected(void) const
{
    return db_connection->isOpen();
}

void SQLiteAdapter::closeConnection(void)
{
    db_connection->close();
}

}
