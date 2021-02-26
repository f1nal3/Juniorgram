#include "../Headers/DataBasePostgreSQL.h"

namespace DBPostgre
{

	void PostgreColumnOperations::addNewColumn(const std::string_view& tableName,
                                               const std::string_view& newColumn,
                                               const std::string_view& columnType) const
    {
        pqxx::work work{ this->m_dbInstance->getConnection() };
    
        work.exec0("ALTER TABLE " + work.esc(tableName) + " ADD COLUMN " + work.esc(newColumn) + ' ' +
                   work.esc(columnType) + ';');
    
        work.commit();
    }
    
    void PostgreColumnOperations::deleteColumn(const std::string_view& tableName,
                                               const std::string_view& columnName) const
    {
        pqxx::work work{ this->m_dbInstance->getConnection() };
    
        work.exec0("ALTER TABLE " + work.esc(tableName) + " DROP COLUMN " + work.esc(columnName) + ';');
    
        work.commit();
    }

}
