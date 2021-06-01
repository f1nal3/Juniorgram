#include "SQLCipherAdapter.hpp"

namespace DataAccess
{
std::shared_ptr<SQLCipherAdapter> SQLCipherAdapter::Instance(const std::string_view& options)
{
    /*QString dbname  = 
    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITECIPHER", "connection_db");
    db.setDatabaseName(dbname);*/

    //qDebug() << QSqlDatabase::drivers();
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLCIPHER");
    //db.setDatabaseName(QDir(tmpDir.path()).absoluteFilePath("client.db"));
    //if (!db.open()) qFatal("Could not access database.");
    //QSqlQuery query(db);
    //query.exec("PRAGMA key = 'secretkey';");


    std::size_t l  = options.length();

    l = 10;

    /*QVERIFY2(QSqlDatabase::isDriverAvailable("QSQLCIPHER"), "QSQLCIPHER driver not found.");*/

    //QString dbname  = QDir(tmpDir.path()).absoluteFilePath();
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLCIPHER", "db");
    //db.setDatabaseName("test.db3");

    return msp_instance;
}

std::optional<std::any> SQLCipherAdapter::query(const std::string_view& query)
{
    return std::optional<std::any>(query);
}

/*pqxx::connection&*/void SQLCipherAdapter::getConnection(void) { return; }

bool SQLCipherAdapter::isConnected(void) const { return false; }

void SQLCipherAdapter::closeConnection(void) {}
}  // namespace DataAccess
