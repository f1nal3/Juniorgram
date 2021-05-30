#include "QSQLCipherAdapter.hpp"

namespace DataAccess
{
std::shared_ptr<QSQLCipherAdapter> QSQLCipherAdapter::Instance(/*const std::string_view& options*/)
{
    QString dbname  = QDir(tmpDir.path()).absoluteFilePath("test.db");
    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITECIPHER", "db");
    db.setDatabaseName(dbname);

   /* std::size_t l  = options.length();*/

    return msp_instance;
}

std::optional<std::any> QSQLCipherAdapter::query(const std::string_view& query)
{
    return std::optional<std::any>(query);
}

/*pqxx::connection&*/void QSQLCipherAdapter::getConnection(void) { return; }

bool QSQLCipherAdapter::isConnected(void) const { return false; }

void QSQLCipherAdapter::closeConnection(void) {}
}  // namespace DataAccess
