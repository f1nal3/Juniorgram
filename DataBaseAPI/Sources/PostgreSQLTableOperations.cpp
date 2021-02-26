#include "../Headers/DataBasePostgreSQL.h"

namespace DBPostgre
{

    void PostgreTableOperations::createTable(const std::string_view& tableName,
                                             const std::string_view& tableFields) const
    {
        pqxx::work work{this->m_dbInstance->getConnection()};
    
        work.exec0("CREATE TABLE IF NOT EXISTS " + work.esc(tableName) + "( " + work.esc(tableFields) + " );");
    
        work.commit();
    }
    
    void PostgreTableOperations::deleteTable(const std::string_view& tableName) const
    {
        pqxx::work work{this->m_dbInstance->getConnection()};
    
        work.exec0("DROP TABLE IF EXISTS " + work.esc(tableName) + ';');
    
        work.commit();
    }

}
