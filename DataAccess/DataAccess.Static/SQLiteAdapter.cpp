#include "SQLiteAdapter.hpp"

namespace DataAccess
{

SQLiteAdapter::SQLiteAdapter()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("SQLiteDB.db");
    db.open();
}

}
