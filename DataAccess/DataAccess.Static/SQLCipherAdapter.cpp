#include "SQLCipherAdapter.hpp"

namespace DataAccess
{
enum class TableExistance : std::uint16_t
{
    nonExistent = 0,
    exists      = 1
};

suppressWarning(4100, Init) 
static int callback(void* data, int argc, char** argv, char** azColName)
{
    auto &dataAlias = *reinterpret_cast<std::vector<std::string>*>(data);

    std::generate_n(std::back_inserter(dataAlias), argc, [i = 0, &argv]() mutable {
        return argv[i++];
    });

    return 0;
}
restoreWarning 

void sqlite3_deleter::operator()(sqlite3* sql) { sqlite3_close(sql); }

sqlite3_ptr make_sqlite3(const std::string_view& dbName)
{
    sqlite3* db = nullptr;

    if (sqlite3_open(dbName.data(), &db) != SQLITE_OK)
    {
        throw Utility::OperationDBException("Failed to open sqlite!", __FILE__, __LINE__);
    }

    suppressWarning(4267, Init) const std::string& hashedMAC =
        Hashing::getSHA512HashingValue(Utility::MACAddressUtility::GetMACAddress());

    sqlite3_key(db, hashedMAC.c_str(), hashedMAC.size());
    restoreWarning

        std::unique_ptr<std::vector<std::string>>
            isExists = std::make_unique<std::vector<std::string>>();

    sqlite3_exec(
        db,
        "SELECT count(name) FROM sqlite_master WHERE type = 'table' AND name = 'refresh_tokens';",
        callback, isExists.get(), 0);

    if (std::stoi(isExists.get()->front()) == (std::uint16_t)TableExistance::nonExistent)
    {
        sqlite3_exec(db, "CREATE TABLE refresh_tokens(refresh_token TEXT NOT NULL);", 0, 0, 0);
    }

  return sqlite3_ptr(db);
}

SQLCipherAdapter::~SQLCipherAdapter() { closeConnection(); }

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
    std::scoped_lock<std::mutex> lock(mStaticMutex);

    if (this->isConnected())
    {
        std::unique_ptr<std::vector<std::string>> res =
            std::make_unique<std::vector<std::string>>();

        char* errMsg = NULL; //take it easy, this is common aproach to use char* in this case

        sqlite3_exec(mDB.get(), "BEGIN TRANSACTION;", NULL, NULL, NULL);

        if (sqlite3_exec(mDB.get(), query.data(), callback, res.get(), &errMsg) != SQLITE_OK)
        {
            std::cout << "Can't execute query, error: " << errMsg;
        }
          
        sqlite3_exec(mDB.get(), "END TRANSACTION;", NULL, NULL, NULL);

        if (!res.get()->empty())
        {
            return std::optional{std::any{*res}};   
        }
    }
    else
    {
        throw Utility::OperationDBException("The connection to the base was lost!", __FILE__,
                                            __LINE__);
    }

    return std::nullopt;
}

bool SQLCipherAdapter::isConnected(void) const
{ 
    if (mDB.get() != nullptr)
        return true;
    else
        return false;
}

void SQLCipherAdapter::closeConnection(void)
{
    mspInstance.reset();
    mDB.reset();
}

}  // namespace DataAccess
