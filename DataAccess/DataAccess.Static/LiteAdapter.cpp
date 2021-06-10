#include "LiteAdapter.hpp"

namespace DataAccess
{

LiteAdapter::LiteAdapter(const QString& path)
{
    db_connection = std::make_unique<QSqlDatabase>();
    *db_connection = QSqlDatabase::addDatabase("QSQlITE");
    db_connection->setDatabaseName(path);
}

std::shared_ptr<LiteAdapter> LiteAdapter::Instance(const QString &path)
{
    std::scoped_lock<std::mutex> lock(mtx);
    if(sqlite_instance == nullptr)
    {
        sqlite_instance = std::shared_ptr<LiteAdapter>(new LiteAdapter(path));
    }
    return sqlite_instance;
}

std::optional<std::any> LiteAdapter::query(const std::string_view &query)
{
    std::scoped_lock<std::mutex> lock(mtx);

    if(this->isConnected())
    {
        QSqlQuery sqlite_query{ *db_connection };
        auto res = sqlite_query.exec(QString::fromStdString(std::string(query)));

        if(res)
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
