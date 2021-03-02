#pragma once
#include <mutex>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

#include "DataAccess/IDB.hpp"

namespace DBPostgre
{
    // Thread safe singleton.
    class PostgreSQL
    {
    private:

        static std::mutex sm_static_mutex;
        static std::shared_ptr<PostgreSQL> spm_instance;

        std::mutex m_query_mutex;
        pqxx::connection m_connection;

    protected:

        PostgreSQL(const std::string_view& options)
            : m_connection{ pqxx::zview(options) } {}

    public:

        PostgreSQL(const PostgreSQL& other) = delete;
        PostgreSQL& operator = (const PostgreSQL& other) = delete;

        PostgreSQL(PostgreSQL&& other) = delete;
        PostgreSQL& operator = (PostgreSQL&& other) = delete;

    public:
        
        static std::shared_ptr<PostgreSQL> getPostgre(const std::string_view& options = "")
        {
            std::lock_guard<std::mutex> lock(sm_static_mutex);

            if (spm_instance == nullptr)
            {
                if (options.empty())
                {  
                    throw pqxx::broken_connection(
                        "Instance didn't create because options are incorrect!");
                }

                spm_instance = std::shared_ptr<PostgreSQL>(new PostgreSQL(options));
            }

            return spm_instance;
        }

        pqxx::connection& getConnection()
        {
            return this->m_connection;
        }

        bool isConnected() const
        {
            return this->m_connection.is_open();
        }

        std::optional<pqxx::result> query(const std::string_view& query)
        {
            std::lock_guard<std::mutex> lock(this->m_query_mutex);

            pqxx::work work{ this->m_connection };

            auto res = work.exec(query);

            work.commit();

            if (!res.empty())
                return std::optional(res);

            return std::nullopt;
        }

    };

    std::mutex PostgreSQL::sm_static_mutex{};
    std::shared_ptr<PostgreSQL> PostgreSQL::spm_instance{};

// ============================================================================================

    class PostgreTableOperations : public DataBases::IDataBaseTableOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_dbInstanse;

    public:

        PostgreTableOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_dbInstanse{ postgre } {}


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
                         const std::string_view& tableFields) const override
        {
            pqxx::work work{ this->m_dbInstanse->getConnection() };

            work.exec0("CREATE TABLE IF NOT EXISTS " + work.esc(tableName) + 
                         "( " + work.esc(tableFields) + " );");

            work.commit();
        }

        void deleteTable(const std::string_view& tableName) const override
        {
            pqxx::work work{ this->m_dbInstanse->getConnection() };

            work.exec0("DROP TABLE IF EXISTS " + work.esc(tableName) + ';');

            work.commit();
        }

    };

    class PostgreFieldOperations : public DataBases::IDataBaseFieldOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_dbInstanse;

    public:

        PostgreFieldOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_dbInstanse{ postgre } {} 


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
                                          const std::string& additional = {},
                                          const pqxx::result::size_type numberOfTheRows = -1) const 
        { 
            pqxx::nontransaction work{ this->m_dbInstanse->getConnection() };

            const std::string query = "SELECT " + work.esc(columnsNames) + " FROM " +
                                     work.esc(tableName) + ' ' + work.esc(additional) + ';';
            if (numberOfTheRows == -1) 
                return work.exec(query);

            return work.exec_n(numberOfTheRows, query);
        }

        // INSERT INTO 'tableName'(columnsNames) VALUES (...)
        // columnsNames = 'columnName1, columnName2, ...'
        void insert(const std::string_view& tableName,
                    const std::string& data,
                    const std::string_view& columnsNames = {},
                    const std::string_view& additional = {}) const override
        {
            pqxx::work work{ this->m_dbInstanse->getConnection() };

            std::string columns{};
            if (!columnsNames.empty())
			   	columns = '(' + work.esc(columnsNames) + ')';

            work.exec0("INSERT INTO " + work.esc(tableName) + columns + " VALUES (" +
                         data + ") " + work.esc(additional) + ';');

            work.commit();
        }

        void update(const std::string_view& tableName,
                    const std::string_view& columnsNamesAndNewData,
                    const std::string_view& additional = {}) const override
        {
            pqxx::work work{ this->m_dbInstanse->getConnection() };

            work.exec0("UPDATE " + work.esc(tableName) + " SET " +
                work.esc(columnsNamesAndNewData) + work.esc(additional) + ';');

            work.commit();
        }

        // !!! IF YOU DON'T USE 'additional' YOU MAY CLEAR ALL TABLE !!!
        void del(const std::string_view& tableName,
                 const std::string_view& additional = {}) const override
        {
            pqxx::work work{this->m_dbInstanse->getConnection()};

            work.exec0("DELETE FROM " + work.esc(tableName) + work.esc(additional) + ';');

            work.commit();
        }

        // SELECT * FROM 'tableName' WHERE 'column' = 'data'
        bool isExist(const std::string_view& tableName,
                     const std::string_view& column,
                     const std::string& data) const override
        {
            pqxx::nontransaction work{this->m_dbInstanse->getConnection()};

            try
            {
                pqxx::row res = work.exec1("SELECT * FROM " +
                    work.esc(tableName) + " WHERE " + work.esc(column) + 
                    " = " + work.esc(data) + ';');

                if (!res.empty())
				   	return true;
            }
            catch (...)
            {
                return false; 
            }

            return false;   
        }

    };

    class PostgreColumnOperations : public DataBases::IDataBaseColumnOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_dbInstanse;

    public:

        PostgreColumnOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_dbInstanse{ postgre } {}
        

    public:

        void addNewColumn(const std::string_view& tableName,
                          const std::string_view& newColumn,
                          const std::string_view& columnType) const override
        {
            pqxx::work work{ this->m_dbInstanse->getConnection() };

            work.exec0("ALTER TABLE " + work.esc(tableName) + " ADD COLUMN " + 
                work.esc(newColumn) + ' ' + work.esc(columnType) + ';');

            work.commit();
        }

        void deleteColumn(const std::string_view& tableName,
                          const std::string_view& columnName) const override
        {
            pqxx::work work{this->m_dbInstanse->getConnection()};

            work.exec0("ALTER TABLE " + work.esc(tableName) + " DROP COLUMN " + 
                work.esc(columnName) + ';');
            
            work.commit();
        }

    };
}

