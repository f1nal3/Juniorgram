//#include "LiteAdapter.hpp"
//
//namespace DataAccess
//{
//LiteAdapter::LiteAdapter(const std::string_view& db_name)
//{
//    _dbConnection  = std::make_unique<QSqlDatabase>();
//    *_dbConnection = QSqlDatabase::addDatabase("QSQLITE");
//    _dbConnection->setDatabaseName(QString::fromStdString(std::string(db_name)));
//    _dbConnection->open();
//}
//
//std::shared_ptr<LiteAdapter> LiteAdapter::Instance(const std::string_view& db_name)
//{
//    std::scoped_lock<std::mutex> lock(_mtx);
//    if (_sqliteInstance == nullptr)
//    {
//        _sqliteInstance = std::shared_ptr<LiteAdapter>(new LiteAdapter(db_name));
//    }
//    return _sqliteInstance;
//}
//
//std::optional<std::any> LiteAdapter::query(const std::string_view& query)
//{
//    std::scoped_lock<std::mutex> lock(_queryMTX);
//
//    if (this->isConnected())
//    {
//        QSqlQuery sqlite_query(*_dbConnection);
//
//        auto result = sqlite_query.exec(QString::fromStdString(std::string(query)));
//        if (!result)
//        {
//            qDebug() << sqlite_query.lastError().text();
//        }
//
//        if (sqlite_query.isSelect())
//        {
//            return std::optional{std::any{sqlite_query}};
//        }
//    }
//    else
//    {
//        throw Utility::OperationDBException("Connection with database was released!", __FILE__, __LINE__);
//    }
//
//    return std::nullopt;
//}
//
//bool LiteAdapter::isConnected(void) const { return _dbConnection->isOpen(); }
//
//void LiteAdapter::closeConnection(void) { _dbConnection->close(); }
//
//}  /// namespace DataAccess
