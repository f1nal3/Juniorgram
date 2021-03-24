#include "PostgreOperations.hpp"

namespace DataAccess
{

    void PostgreTableOperations::createTable(const std::string_view& tableName,
                                             const std::string_view& tableFields) const noexcept
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };
    
        work.exec0("CREATE TABLE IF NOT EXISTS " + work.esc(tableName) + "( " + work.esc(tableFields) +
                   " );");
    
        work.commit();
    }
    
    void PostgreTableOperations::deleteTable(const std::string_view& tableName) const noexcept
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };
    
        work.exec0("DROP TABLE IF EXISTS " + work.esc(tableName) + ';');
    
        work.commit();
    }

    

    [[nodiscard]] pqxx::result PostgreFieldOperations::select(
                                      const std::string& tableName, 
                                      const std::string& columnsNames,
                                      const std::string& additional                 = {},
                                      const pqxx::result::size_type numberOfTheRows = -1) const
    {
        pqxx::nontransaction work{ m_dataBaseInstance->getConnection()};

        const std::string query = "SELECT " + work.esc(columnsNames) + " FROM " +
                                  work.esc(tableName) + ' ' + work.esc(additional) + ';';
        if (numberOfTheRows == -1) return work.exec(query);

        return work.exec_n(numberOfTheRows, query);
    }

    void PostgreFieldOperations::insert(const std::string_view& tableName,
                                        const std::string& data,
                                        const std::string_view& columnsNames = {},
                                        const std::string_view& additional   = {}) const
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        std::string columns{};
        if (!columnsNames.empty()) columns = '(' + work.esc(columnsNames) + ')';

        work.exec0("INSERT INTO " + work.esc(tableName) + columns + " VALUES (" + data + ") " +
                   work.esc(additional) + ';');

        work.commit();
    }

    void PostgreFieldOperations::update(const std::string_view& tableName,
                                        const std::string_view& columnsNamesAndNewData,
                                        const std::string_view& additional = {}) const
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        work.exec0("UPDATE " + work.esc(tableName) + " SET " + work.esc(columnsNamesAndNewData) +
                   work.esc(additional) + ';');

        work.commit();
    }

    void PostgreFieldOperations::del(const std::string_view& tableName,
                                     const std::string_view& additional = {}) const
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };
        
        work.exec0("DELETE FROM " + work.esc(tableName) + work.esc(additional) + ';');

        work.commit();
    }

    bool PostgreFieldOperations::isExist(const std::string_view& tableName,
                                         const std::string_view& column,
                                         const std::string& data) const
    {
        pqxx::nontransaction work{ m_dataBaseInstance->getConnection() };

        try
        {
            pqxx::row res = work.exec1("SELECT * FROM " + work.esc(tableName) + " WHERE " +
                                       work.esc(column) + " = " + work.esc(data) + ';');

            if (!res.empty()) return true;
        }
        catch (...)
        {
            return false;
        }

        return false;
    }



    void PostgreColumnOperations::addNewColumn(const std::string_view& tableName,
                                             const std::string_view& newColumn,
                      const std::string_view& columnType) const 
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        work.exec0("ALTER TABLE " + work.esc(tableName) + " ADD COLUMN " + work.esc(newColumn) +
                   ' ' + work.esc(columnType) + ';');

        work.commit();
    }

    void PostgreColumnOperations::deleteColumn(const std::string_view& tableName,
                      const std::string_view& columnName) const 
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        work.exec0("ALTER TABLE " + work.esc(tableName) + " DROP COLUMN " + work.esc(columnName) +
                   ';');

        work.commit();
    }

}
