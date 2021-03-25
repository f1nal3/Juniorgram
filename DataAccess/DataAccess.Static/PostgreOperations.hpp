#pragma once
#include <memory>
#include <sstream>

#include <pqxx/pqxx>

#include "PostgreAdapter.hpp"
#include "DataAccess/IDB.hpp"

namespace DataAccess
{

    std::string toSqlType(std::string& type, std::uint16_t length = {});

    class PostgreTableOperations : public IDataBaseTableOperations
    {
    private:

        std::shared_ptr<PostgreAdapter> m_dataBaseInstance;

    public:

        PostgreTableOperations(const std::shared_ptr<PostgreAdapter> postgre)
            : m_dataBaseInstance{ postgre } {}


        PostgreTableOperations(const PostgreTableOperations& other) = delete;
        PostgreTableOperations& operator = (const PostgreTableOperations& other) = delete;

        PostgreTableOperations(PostgreTableOperations&& other) = delete;
        PostgreTableOperations& operator = (PostgreTableOperations&& other) = delete;

    public:

        /// You need to write 'tableFields' like:
        ///  columnName1 type,
        ///  columaName2 type,
        ///  N-----------------N
        ///  columnNameN type
        void createTable(const std::string&  tableName,
                         const typeDataName& tableFields) const override;

        void deleteTable(const std::string&  tableName) const override;

    };

    class PostgreFieldOperations : public IDataBaseFieldOperations
    {
    private:

        std::shared_ptr<PostgreAdapter> m_dataBaseInstance;

    public:

        PostgreFieldOperations(const std::shared_ptr<PostgreAdapter> postgre)
            : m_dataBaseInstance{ postgre } {} 


        PostgreFieldOperations(const PostgreFieldOperations& other) = delete;
        PostgreFieldOperations& operator = (const PostgreFieldOperations& other) = delete;

        PostgreFieldOperations(PostgreFieldOperations&& other) = delete;
        PostgreFieldOperations& operator = (PostgreFieldOperations&& other) = delete;

    public:

        /// additional => "WHERE", "ORDER BY", ...
        /// If you don't need 'additional', but need 'numberOfTheColumns' use = "".
        /// numberOfTheColumns = "columnName1, columnName2, ..."
        [[nodiscard]] pqxx::result select(const std::string& tableName,
                                          const std::string& columnsNames,
                                          const std::string& additional                 = {},
                                          const pqxx::result::size_type numberOfTheRows = -1) const;

        /// INSERT INTO 'tableName'(columnsNames) VALUES (...)
        /// columnsNames = 'columnName1, columnName2, ...'
        void insert(const std::string_view& tableName,
                    const columnData& columnsAndData = {},
                    const std::string_view& additional   = {}) const override;

        void update(const std::string_view& tableName,
                    const columnData& columnsAndNewData,
                    const std::string_view& additional = {}) const override;

        /// !!! IF YOU DON'T USE 'additional' YOU MAY CLEAR ALL TABLE !!!
        void del(const std::string_view& tableName,
                 const std::string_view& additional = {}) const override;

        /// SELECT * FROM 'tableName' WHERE 'column' = 'data'
        bool isExist(const std::string_view& tableName, 
                     const std::string& column,
                     const std::string& data) const override;

    };

    class PostgreColumnOperations : public IDataBaseColumnOperations
    {
    private:

        std::shared_ptr<PostgreAdapter> m_dataBaseInstance;

    public:

        PostgreColumnOperations(const std::shared_ptr<PostgreAdapter> postgre)
            : m_dataBaseInstance{ postgre } {}
        

    public:

        void addNewColumn(const std::string_view& tableName, const std::string_view& newColumn,
                          const std::string_view& columnType) const override;

        void deleteColumn(const std::string_view& tableName,
                          const std::string_view& columnName) const override;

    };
}

