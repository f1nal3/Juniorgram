#include "PostgreOperations.hpp"

namespace DataAccess
{

    std::string toSqlType(std::string& type, std::uint16_t length = {}) 
    { 
        if (type == "int")
        {
            return "integer";
        }
        else if (type == "long long")
        {
            return "bigint";
        }
        else if (type == "char" || type == "string")
        {
            if (length != 0)
            {
                return "varchar(" + std::to_string(length) + ")";
            }
        }
        else if (type == "float" || type == "double")
        {
            return "real";
        }
        else if (type == "bool")
        {
            return "boolean";        
        }

        throw std::runtime_error("Inccorect type!" + __LINE__);
    }

    void PostgreTableOperations::createTable(const std::string& tableName,
                                             const typeDataName& tableFields) const noexcept
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };
    
        std::stringstream fields{};
        for (auto[type, name] : tableFields)
        {
            fields << " " << toSqlType(type) << " " << name;
        }

        work.exec0("create table if exists (" + fields.str() + ");");
        
        work.commit();
    }
    
    void PostgreTableOperations::deleteTable(const std::string& tableName) const noexcept
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };
    
        work.exec0("drop table if exists " + tableName + ';');
    
        work.commit();
    }

    

    [[nodiscard]] pqxx::result PostgreFieldOperations::select(
                                      const std::string& tableName, 
                                      const std::string& columnsNames,
                                      const std::string& additional                 = {},
                                      const pqxx::result::size_type numberOfTheRows = -1) const
    {
        pqxx::nontransaction work{ m_dataBaseInstance->getConnection()};

        const std::string query =
            "select " + columnsNames + " from " + tableName + ' ' + additional + ';';

        if (numberOfTheRows == -1) 
            return work.exec(query);

        return work.exec_n(numberOfTheRows, query);
    }

    void PostgreFieldOperations::insert(const std::string_view& tableName,
                                        const columnData& columnsNames = {},
                                        const std::string_view& additional   = {}) const
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        std::stringstream columns{}, tempData{};
        for (auto&[column, data] : columnsNames)
        {
            columns  << " " << column << ',';
            tempData << " " << data   << ','; 
        }

        work.exec0("insert into " + work.esc(tableName) + " (" + columns.str() + ") " + "values(" +
                   tempData.str() + ')' + work.esc(additional) + ';');

        work.commit();
    }

    void PostgreFieldOperations::update(const std::string_view& tableName,
                                        const columnData& columnsAndNewData,
                                        const std::string_view& additional = {}) const
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        for (auto&[column, data] : columnsAndNewData)
        {
            work.exec0("update " + work.esc(tableName) + " set " + column + " = " + data + ' ' +
                       work.esc(additional) + ';');
        }

        work.commit();
    }

    void PostgreFieldOperations::del(const std::string_view& tableName,
                                     const std::string_view& additional = {}) const
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };
        
        work.exec0("delete from " + work.esc(tableName) + work.esc(additional) + ';');

        work.commit();
    }

    bool PostgreFieldOperations::isExist(const std::string_view& tableName,
                                         const std::string& column,
                                         const std::string& data) const
    {
        pqxx::nontransaction work{ m_dataBaseInstance->getConnection() };

        try
        {
            pqxx::row res = work.exec1("select * from " + work.esc(tableName) + " where " + column +
                                       " = " + work.esc(data) + ';');

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

        work.exec0("alter table " + work.esc(tableName) + " add column " + work.esc(newColumn) +
                   ' ' + work.esc(columnType) + ';');

        work.commit();
    }

    void PostgreColumnOperations::deleteColumn(const std::string_view& tableName,
                      const std::string_view& columnName) const 
    {
        pqxx::work work{ m_dataBaseInstance->getConnection() };

        work.exec0("alter table " + work.esc(tableName) + " drop column " + work.esc(columnName) +
                   ';');

        work.commit();
    }

}
