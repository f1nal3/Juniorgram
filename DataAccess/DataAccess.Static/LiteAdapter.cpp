#include "LiteAdapter.hpp"

namespace DataAccess
{

LiteAdapter::LiteAdapter(const std::string_view& db_name)
{
    db_connection = std::make_unique<QSqlDatabase>();
    *db_connection = QSqlDatabase::addDatabase("QSQLITE");
    db_connection->setDatabaseName(QString::fromStdString(std::string(db_name)));
    db_connection->open();
}

std::shared_ptr<LiteAdapter> LiteAdapter::Instance(const std::string_view& db_name)
{
    std::scoped_lock<std::mutex> lock(mtx);
    if(sqlite_instance == nullptr)
    {
        sqlite_instance = std::shared_ptr<LiteAdapter>(new LiteAdapter(db_name));
    }
    return sqlite_instance;
}

std::optional<std::any> LiteAdapter::query(const std::string_view &query)
{
    std::scoped_lock<std::mutex> lock(query_mtx);

    if(this->isConnected())
    {
        QSqlQuery sqlite_query{ *db_connection };
        sqlite_query.exec(QString::fromStdString(std::string(query)));

        if(sqlite_query.isSelect())
        {
            return std::optional{ std::any { sqlite_query } };
        }

    }
    else
    {
        throw Utility::OperationDBException("Connection with database was released!", __FILE__, __LINE__);
    }

    return std::nullopt;
}

bool LiteAdapter::isConnected(void) const
{
    return db_connection->isOpen();
}

void LiteAdapter::closeConnection(void)
{
    db_connection->close();
}

}
