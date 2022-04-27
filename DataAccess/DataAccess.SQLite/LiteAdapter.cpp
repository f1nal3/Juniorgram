#include "LiteAdapter.hpp"

#include <QDebug>
#include <QSqlError>

namespace DataAccess
{
LiteAdapter::LiteAdapter(const std::string_view& db_name)
{
    _DBConnection  = std::make_unique<QSqlDatabase>();
    *_DBConnection = QSqlDatabase::addDatabase("QSQLITE");
    _DBConnection->setDatabaseName(QString::fromStdString(std::string(db_name)));
    _DBConnection->open();
}

std::shared_ptr<LiteAdapter> LiteAdapter::Instance(const std::string_view& db_name)
{
    std::scoped_lock<std::mutex> lock(_mtx);
    if (_SQLiteInstance == nullptr)
    {
        _SQLiteInstance = std::shared_ptr<LiteAdapter>(new LiteAdapter(db_name));
    }
    return _SQLiteInstance;
}

std::optional<std::any> LiteAdapter::query(const std::string_view& query)
{
    std::scoped_lock<std::mutex> lock(_queryMTX);

    if (this->isConnected())
    {
        QSqlQuery sqlite_query(*_DBConnection);

        auto result = sqlite_query.exec(QString::fromStdString(std::string(query)));
        if (!result)
        {
            qDebug() << sqlite_query.lastError().text();
        }

        if (sqlite_query.isSelect())
        {
            return std::optional{std::any{sqlite_query}};
        }
    }
    else
    {
        throw Utility::OperationDBException("Connection with database was released!", __FILE__, __LINE__);
    }

    return std::nullopt;
}

bool LiteAdapter::isConnected(void) const { return _DBConnection->isOpen(); }

void LiteAdapter::closeConnection(void) { _DBConnection->close(); }

}  /// namespace DataAccess
