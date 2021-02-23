#pragma once
#include <mutex>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

#include "IDB.hpp"

namespace DBPostgre
{
    // Thread safe singleton.
    class PostgreSQL
    {
        template <class _Ty, class... _Types>
        friend void std::_Construct_in_place(_Ty& _Obj, _Types&&... _Args);

    private:

        static std::mutex sm_static_mutex;
        static std::shared_ptr<PostgreSQL> spm_instance;

        std::mutex m_quary_mutex{};

        pqxx::connection m_con;

    private:

        PostgreSQL(const std::string_view& options)
            : m_con{ pqxx::zview(options) } {}

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

                spm_instance = std::make_shared<PostgreSQL>(options);
            }

            return spm_instance;
        }

        pqxx::connection& getConnection()
        {
            return this->m_con;
        }

        bool isConnected() const
        {
            return this->m_con.is_open();
        }

        std::optional<pqxx::result> quary(const std::string_view& quary)
        {
            std::lock_guard<std::mutex> lock(this->m_quary_mutex);

            pqxx::work work{ this->m_con };

            auto res = work.exec(quary);

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

        std::shared_ptr<PostgreSQL> m_db{};
        mutable pqxx::work m_work;

    public:

        PostgreTableOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_db{ postgre }, m_work{ this->m_db->getConnection() } {}


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
            m_work.exec0("CREATE TABLE IF NOT EXISTS " + m_work.esc(tableName) + 
                         "( " + m_work.esc(tableFields) + " );");

            m_work.commit();
        }

        void deleteTable(const std::string_view& tableName) const override
        {
            m_work.exec0("DROP TABLE IF EXISTS " + m_work.esc(tableName) + ';');

            m_work.commit();
        }

    };

    class PostgreFieldOperations : public DataBases::IDataBaseFieldOperations
    {
    private:

        std::shared_ptr<PostgreSQL> m_db{};
        mutable pqxx::work m_work;

    public:

        PostgreFieldOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_db{ postgre }, m_work{ this->m_db->getConnection() } {} 


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
            const std::string quary = "SELECT " + m_work.esc(columnsNames) + " FROM " +
                                     m_work.esc(tableName) + ' ' + m_work.esc(additional) + ';';
            pqxx::result res{};

            if (numberOfTheRows == -1) 
                res = m_work.exec(quary);

           res = m_work.exec_n(numberOfTheRows, pqxx::zview(quary));

           m_work.commit();

           return res;
        }

        // INSERT INTO 'tableName'(columnsNames) VALUES (...)
        // columnsNames = 'columnName1, columnName2, ...'
        void insert(const std::string_view& tableName,
                    const std::string& data,
                    const std::string_view& columnsNames = {},
                    const std::string_view& additional = {}) const override
        {
            std::string columns{};
            if (!columnsNames.empty()) columns = '(' + m_work.esc(columnsNames) + ')';

            m_work.exec0("INSERT INTO " + m_work.esc(tableName) + columns + " VALUES (" +
                         data + ") " + m_work.esc(additional) + ';');

            m_work.commit();
        }

        void update(const std::string_view& tableName,
                    const std::string_view& columnsNamesAndNewData,
                    const std::string_view& additional = {}) const override
        {
            m_work.exec0("UPDATE " + m_work.esc(tableName) + " SET " +
                m_work.esc(columnsNamesAndNewData) + m_work.esc(additional) + ';');

            m_work.commit();
        }

        // !!! IF YOU DON'T USE 'additional' YOU MAY CLEAR ALL TABLE !!!
        void del(const std::string_view& tableName,
                 const std::string_view& additional = {}) const override
        {
            m_work.exec0("DELETE FROM " + m_work.esc(tableName) + m_work.esc(additional) + ';');

            m_work.commit();
        }

        // KOSTIL. NADO ISPRAVIT'!!!!
        // SELECT * FROM 'tableName' WHERE 'column' = 'data'
        bool isExist(const std::string_view& tableName,
                     const std::string_view& column,
                     const std::string& data) const override
        {
            try
            {
                pqxx::row res = m_work.exec1("SELECT * FROM " +
                    m_work.esc(tableName) + " WHERE " + m_work.esc(column) + 
                    " = " + m_work.esc(data) + ';');

                m_work.commit();

                if (!res.empty()) return true;
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

        std::shared_ptr<PostgreSQL> m_db{};
        mutable pqxx::work m_work;   

    public:

        PostgreColumnOperations(const std::shared_ptr<PostgreSQL> postgre)
            : m_db{ postgre }, m_work{ this->m_db->getConnection() } {}
        

    public:

        void addNewColumn(const std::string_view& tableName,
                          const std::string_view& newColumn,
                          const std::string_view& columnType) const override
        {
            m_work.exec0("ALTER TABLE " + m_work.esc(tableName) + " ADD COLUMN " + 
                m_work.esc(newColumn) + ' ' + m_work.esc(columnType) + ';');

            m_work.commit();
        }

        void deleteColumn(const std::string_view& tableName,
                          const std::string_view& columnName) const override
        {
            m_work.exec0("ALTER TABLE " + m_work.esc(tableName) + " DROP COLUMN " + 
                m_work.esc(columnName) + ';');
            
            m_work.commit();
        }

    };
}