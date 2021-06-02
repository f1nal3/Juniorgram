#include "Utility/WarningSuppression.hpp"
#include "SQLCipherAdapter.hpp"
#include <iostream>


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
suppressWarning(4505, Init)
suppressWarning(4100, Init)
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
restoreWarning 
restoreWarning

std::shared_ptr<SQLCipherAdapter> SQLCipherAdapter::Instance(const std::string_view& options)
{
      sqlite3* buffer = nullptr;
    /*  auto mysqlite = sqlite3_open("dbname.db", &buffer);*/

    int rc = sqlite3_open("dbname.db", &buffer);

     std::string pw = "blablablablablablabla";
    suppressWarning(4267, Init)
    if ((rc = sqlite3_key(buffer, pw.data(), pw.length())) != SQLITE_OK)
    {
        printf("failed to key database: error %d\n", rc);
    }
    restoreWarning

    char* sql;

            sql =
               "CREATE TABLE COMPANY("
               "ID INT PRIMARY KEY     NOT NULL,"
               "NAME           TEXT    NOT NULL,"
               "AGE            INT     NOT NULL,"
               "ADDRESS        CHAR(50),"
               "SALARY         REAL );";

       /* Execute SQL statement */

         char* zErrMsg = 0;
            rc            = sqlite3_exec(buffer, sql, callback, 0, &zErrMsg);




   
 /*   if (!isDbEncrypted(db))
    {*/
   /* sqlite3_key(buffer, "", 0);
         suppressWarning(4267, Init) */
  /*  sqlite3_rekey(buffer, pw.data(), pw.length());*/
        /* restoreWarning*/
       /* qInfo() << "Database was encrypted.";*/
  /*  }*/
    //else
    //{
    //    sqlite3_key(db, pw.data(), pw.length());
    //    sqlite3_rekey(db, "", 0);
    //    qInfo() << "Database was decrypted.";
    //}

    sqlite3_close(buffer);

  //  //auto mysqlite   = make_sqlite();
  // /* const char* key = {"222"};*/
  //  sqlite3* buffer = nullptr;
  //  auto mysqlite = sqlite3_open("dbname.db", &buffer);

  //  mysqlite = 10;

  //  
  // 
  //       
  //       char* sql;

  //        sql =
  //           "CREATE TABLE COMPANY("
  //           "ID INT PRIMARY KEY     NOT NULL,"
  //           "NAME           TEXT    NOT NULL,"
  //           "AGE            INT     NOT NULL,"
  //           "ADDRESS        CHAR(50),"
  //           "SALARY         REAL );";

  //   /* Execute SQL statement */

  //     char* zErrMsg = 0;
  //   mysqlite = sqlite3_exec(buffer, sql, callback, 0, &zErrMsg);

  ///* mysqlite = sqlite3_exec(buffer, "ATTACH DATABASE 'sqlliteDB.db' AS encrypted KEY 'testkey'",
  //                           callback, 0, &zErrMsg);*/


  //     suppressWarning(4267, Init) 
  //             sqlite3_key(buffer, "1111", strlen("1111"));
  //   restoreWarning 

  //       sqlite3_close(buffer);

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
