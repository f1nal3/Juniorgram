#include "SQLCipherAdapter.hpp"

namespace DataAccess
{
void sqlite3_deleter::operator()(sqlite3* sql) { sqlite3_close_v2(sql); }

sqlite3_ptr make_sqlite()
{
    {
        sqlite3* buffer = nullptr;
        int err         = sqlite3_open("sqlliteDB.db", &buffer);
        if (err)
        {
            throw std::runtime_error("failed to open sqlite");
        }
        return sqlite3_ptr(buffer);
    }
}

std::shared_ptr<SQLCipherAdapter> SQLCipherAdapter::Instance(const std::string_view& options)
{

    auto mysqlite   = make_sqlite();
   /* const char* key = {"222"};*/

     std::string k  = "gKv>áD8~2a+>4B3Z533";
    k.pop_back();
     sqlite3_key(mysqlite.get(), k.c_str(), (int)k.size());

      k.pop_back();
     k.pop_back();

    //sqlite3_key(mysqlite.get()/*, "sqlliteDB.db"*/, key, sizeof(key));

  /*  std::scoped_lock<std::mutex> lock(ms_static_mutex);

    if (msp_instance == nullptr)
    {
        mspInstance = std::shared_ptr<SQLCipherAdapter>(
            new SQLCipherAdapter(options.empty() ? ms_defaultOptions : options));
    }

    

    try
    {
        Singleton::getInstance().mrc = sqlite3_open("RClients.db", &Singleton::getInstance().mdb);

        if (Singleton::getInstance().mrc == SQLITE_CANTOPEN)
        {
            throw false;
        }
        else if (Singleton::getInstance().mrc == SQLITE_OK)
        {
            isOpen = true;
        }
    }
    catch (bool)
    {
        isOpen = false;
    }

    if (isOpen)
    {
        API::RClientsDB::ResponseParse(10)(Singleton::getInstance().mdb);
    }

*/


    

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

    return mspInstance;
}

std::optional<std::any> SQLCipherAdapter::query(const std::string_view& query)
{
    return std::optional<std::any>(query);
}

/*pqxx::connection&*/void SQLCipherAdapter::getConnection(void) { return; }

bool SQLCipherAdapter::isConnected(void) const { return false; }

void SQLCipherAdapter::closeConnection(void) {}



}  // namespace DataAccess
