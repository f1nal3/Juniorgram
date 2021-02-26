#include "../Headers/DataBasePostgreSQL.h"

namespace DBPostgre
{

	[[nodiscard]] 
    pqxx::result PostgreFieldOperations::select(const std::string& tableName, 
                                                const std::string& columnsNames,
                                                const std::string& additional                 = {},
                                                const pqxx::result::size_type numberOfTheRows = -1) const
    {
        pqxx::nontransaction work{ this->m_dbInstance->getConnection() };

        const std::string quary = "SELECT " + work.esc(columnsNames) + " FROM " + work.esc(tableName) +
                                  ' ' + work.esc(additional) + ';';
        if (numberOfTheRows == -1) 
            return work.exec(quary);

        return work.exec_n(numberOfTheRows, quary);
    }

    void PostgreFieldOperations::insert(const std::string_view& tableName, 
                                        const std::string& data,
                                        const std::string_view& columnsNames = {},
                                        const std::string_view& additional   = {}) const
    {
        pqxx::work work{ this->m_dbInstance->getConnection() };

        std::string columns{};
        if (!columnsNames.empty()) 
            columns = '(' + work.esc(columnsNames) + ')';

        work.exec0("INSERT INTO " + work.esc(tableName) + columns + " VALUES (" + data + ") " +
                   work.esc(additional) + ';');

        work.commit();
    }

    void PostgreFieldOperations::update(const std::string_view& tableName, 
                const std::string_view& columnsNamesAndNewData,
                const std::string_view& additional = {}) const
    {
        pqxx::work work{ this->m_dbInstance->getConnection() };

        work.exec0("UPDATE " + work.esc(tableName) + " SET " + work.esc(columnsNamesAndNewData) +
                   work.esc(additional) + ';');

        work.commit();
    }

    void PostgreFieldOperations::del(const std::string_view& tableName,
                                     const std::string_view& additional = {}) const
    {
        pqxx::work work{ this->m_dbInstance->getConnection() };

        work.exec0("DELETE FROM " + work.esc(tableName) + work.esc(additional) + ';');

        work.commit();
    }

    // SELECT * FROM 'tableName' WHERE 'column' = 'data'
    bool PostgreFieldOperations::isExist(const std::string_view& tableName,
                                         const std::string_view& column,
                                         const std::string& data) const
    {
        pqxx::nontransaction work{ this->m_dbInstance->getConnection() };

        try
        {
            pqxx::row res = work.exec1("SELECT * FROM " + work.esc(tableName) + " WHERE " +
                                       work.esc(column) + " = " + work.esc(data) + ';');

            if (!res.empty()) 
                return true;
        }
        catch (...)
        {
            return false;
        }

        return false;
    }

}
