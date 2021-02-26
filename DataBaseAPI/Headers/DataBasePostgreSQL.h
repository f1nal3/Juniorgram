#pragma once
#include <mutex>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

#include "IDB.h"

namespace DBPostgre
{
    // Thread safe singleton.
    class PostgreSQL
    {
    private:

        static std::mutex sm_static_mutex;
        static std::shared_ptr<PostgreSQL> spm_instance;

        std::mutex m_quary_mutex;
        pqxx::connection m_connection;

    protected:

        PostgreSQL(const std::string_view& options) 
            : m_connection{pqxx::zview(options)} {}

    public:

        PostgreSQL(const PostgreSQL& other) = delete;
        PostgreSQL& operator = (const PostgreSQL& other) = delete;

        PostgreSQL(PostgreSQL&& other) = delete;
        PostgreSQL& operator = (PostgreSQL&& other) = delete;

    public:
        
        static std::shared_ptr<PostgreSQL> getPostgre(const std::string_view& options = "");

        pqxx::connection& getConnection();

        bool isConnected() const;

        std::optional<pqxx::result> quary(const std::string_view& quary);

    };

    std::mutex PostgreSQL::sm_static_mutex{};
    std::shared_ptr<PostgreSQL> PostgreSQL::spm_instance{};

// ============================================================================================

    class PostgreTableOperations : public DataBases::IDataBaseTableOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_dbInstance;

    public:

        PostgreTableOperations(const std::shared_ptr<PostgreSQL> postgre) 
            : m_dbInstance{postgre} {}


        PostgreTableOperations(const PostgreTableOperations& other) = delete;
        PostgreTableOperations& operator = (const PostgreTableOperations& other) = delete;

        PostgreTableOperations(PostgreTableOperations&& other) = delete;
        PostgreTableOperations& operator = (PostgreTableOperations&& other) = delete;

    public:

        // You need to write 'tableFields' like:
        //  columnName1 type,
        //  columaName2 type,
        //  N-----------------N
        //  columnNameN type
        void createTable(const std::string_view& tableName,
                         const std::string_view& tableFields) const override;

        void deleteTable(const std::string_view& tableName) const override;

    };

    class PostgreFieldOperations : public DataBases::IDataBaseFieldOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_dbInstance;

    public:

        PostgreFieldOperations(const std::shared_ptr<PostgreSQL> postgre) 
            : m_dbInstance{postgre} {} 


        PostgreFieldOperations(const PostgreFieldOperations& other) = delete;
        PostgreFieldOperations& operator = (const PostgreFieldOperations& other) = delete;

        PostgreFieldOperations(PostgreFieldOperations&& other) = delete;
        PostgreFieldOperations& operator = (PostgreFieldOperations&& other) = delete;

    public:

        // additional => "WHERE", "ORDER BY", ...
        // If you don't need 'additional', but need 'numberOfTheColumns' use = "".
        // numberOfTheColumns = "columnName1, columnName2, ..."
        [[nodiscard]] pqxx::result select(const std::string& tableName,
                                          const std::string& columnsNames,
                                          const std::string& additional                 = {},
                                          const pqxx::result::size_type numberOfTheRows = -1) const;

        // INSERT INTO 'tableName'(columnsNames) VALUES (...)
        // columnsNames = 'columnName1, columnName2, ...'
        void insert(const std::string_view& tableName, 
                    const std::string& data,
                    const std::string_view& columnsNames = {},
                    const std::string_view& additional   = {}) const override;

        void update(const std::string_view& tableName,
                    const std::string_view& columnsNamesAndNewData,
                    const std::string_view& additional = {}) const override;

        // !!! IF YOU DON'T USE 'additional' YOU MAY CLEAR ALL TABLE !!!
        void del(const std::string_view& tableName,
                 const std::string_view& additional = {}) const override;

        // KOSTIL. NADO ISPRAVIT'!!!!
        // SELECT * FROM 'tableName' WHERE 'column' = 'data'
        bool isExist(const std::string_view& tableName, 
                     const std::string_view& column,
                     const std::string& data) const override;

    };

    class PostgreColumnOperations : public DataBases::IDataBaseColumnOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_dbInstance;

    public:

        PostgreColumnOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_dbInstance{postgre} {}
        

    public:

        void addNewColumn(const std::string_view& tableName, 
                          const std::string_view& newColumn,
                          const std::string_view& columnType) const override;

        void deleteColumn(const std::string_view& tableName,
                          const std::string_view& columnName) const override;

    };

}
