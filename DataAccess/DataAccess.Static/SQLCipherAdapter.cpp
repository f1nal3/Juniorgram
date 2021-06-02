#include "Utility/WarningSuppression.hpp"
#include "SQLCipherAdapter.hpp"
#include <iostream>

// suppressWarning(4505, Init)
// suppressWarning(4100, Init)
// static int callback(void* NotUsed, int argc, char** argv, char** azColName)
//{
//    int i;
//    for (i = 0; i < argc; i++)
//    {
//        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
//}
// restoreWarning
// restoreWarning

namespace DataAccess
{
void sqlite3_deleter::operator()(sqlite3* sql) { sqlite3_close(sql); }

sqlite3_ptr make_sqlite(const std::string_view& dbName)
{
    sqlite3* db = nullptr;
        
    if (sqlite3_open(dbName.data(), &db) != SQLITE_OK)
    {
        throw Utility::OperationDBException("Failed to open sqlite!", __FILE__, __LINE__);
    }

    return sqlite3_ptr(db);
}

std::shared_ptr<SQLCipherAdapter> SQLCipherAdapter::Instance(const std::string_view& settedName)
{
    std::scoped_lock<std::mutex> lock(mStaticMutex);

    if (mspInstance == nullptr)
    {
        mspInstance = std::shared_ptr<SQLCipherAdapter>(
            new SQLCipherAdapter(settedName.empty() ? msDefaultDBName : settedName));
    }

    return mspInstance;
}

std::optional<std::any> SQLCipherAdapter::query(const std::string_view& query)
{
    return std::optional<std::any>(query);
}

/*pqxx::connection&*/ void SQLCipherAdapter::getConnection(void) { return; }

bool SQLCipherAdapter::isConnected(void) const { return false; }

void SQLCipherAdapter::closeConnection(void) { sqlite3_close(mDB.get()); }

}  // namespace DataAccess
