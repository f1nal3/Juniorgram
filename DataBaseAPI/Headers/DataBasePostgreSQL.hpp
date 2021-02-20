#pragma once
#include <mutex>
#include <tuple>
#include <memory>
#include <optional>

#include <pqxx/pqxx>

#include "IDB.hpp"

namespace DBPostgre
{
    // Thread safe singleton.
    class PostgreSQL
    {
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

#ifdef _DEBUG || !NDEBUG
                std::cout << "[DEBUG]: Instance (Postgre) was created succesfully!" << std::endl;
#endif

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
            m_work.exec0("CREATE TABLE IF NOT EXISTS " + 
                         m_work.esc(std::string{ tableName }) + 
                         "( " +
                         m_work.esc(std::string{ tableFields } + " );")
                        );

            m_work.commit();
        }

        void deleteTable(const std::string_view& tableName) const override
        {
            m_work.exec0("DROP TABLE IF EXISTS " + 
                         m_work.esc(std::string{ tableName }) + 
                         ";"
                        );

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
        pqxx::result select(const std::string_view& tableName,
                            const std::string_view& columnsNames,
                            const std::string_view& additional,
                            const pqxx::result::size_type numberOfTheRows = -1) const 
        { 
            const std::string_view quary = "SELECT " + this->m_work.esc(columnsNames) + " FROM " +
                                     this->m_work.esc(tableName) + this->m_work.esc(additional) + ";";
            pqxx::result res{};

            if (numberOfTheRows == -1) 
                res = this->m_work.exec(quary);

           res = this->m_work.exec_n(numberOfTheRows, pqxx::zview(quary));

           this->m_work.commit();

           return res;
        }

        // INSERT INTO 'tableName'(columnsNames) VALUES ...
        // columnsNames = 'columnName1, columnName2, ...'
        void insert(const std::string_view& tableName,
                    const std::string_view& data,
                    const std::string_view& columnsNames = {},
                    const std::string_view& additional = {}) const override
        {
            const std::string_view columns = (columnsNames == "" ? "(" + m_work.esc(columnsNames) + ")" : ""); 

            m_work.exec0("INSERT INTO " + m_work.esc(tableName)
                + m_work.esc(columns) + " VALUES " + m_work.esc(data) + ' ' + m_work.esc(additional) + ';');

            m_work.commit();
        }

        void update(const std::string_view& quary) const override
        {
            m_work.exec0(pqxx::zview(quary));
        }

        void del(const std::string_view& quary) const override
        {
            m_work.exec0(pqxx::zview(quary));
        }

        bool isExist(const std::string_view& quary) const override
        {
            pqxx::row res = m_work.exec1(pqxx::zview(quary));

            if (!res.empty())
                return true;

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

        void addNewColumn(const std::string_view& quary) const override
        {
            m_work.exec0(pqxx::zview(quary));

            m_work.commit();
        }

        void deleteColumn(const std::string_view& quary) const override
        {
            m_work.exec0(pqxx::zview(quary));
            
            m_work.commit();
        }

    };
}