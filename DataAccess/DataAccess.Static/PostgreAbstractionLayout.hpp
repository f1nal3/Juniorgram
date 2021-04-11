#pragma once

#include "PostgreAdapter.hpp"

#include <Utility/Exception.hpp>

namespace DataAccess
{
    class Table
    {
    public:
        explicit Table(const char* tableName) 
            : m_tableName{tableName}, m_query{false},
              m_choosedOp{Operation::NOTHING} 
        {
            mp_postgre = PostgreAdapter::getPostgre(
                "dbname=postgres user=postgres hostaddr=127.0.0.1 port=5432");
        }
        Table(const std::string& tableName)
            : Table(tableName.c_str()) {}

        virtual ~Table() = default;
    
    public:
        Table() = delete;
    
        Table(const Table&) = delete;
        Table(Table&&)      = delete;
    
        Table& operator=(const Table&) = delete;
        Table& operator=(Table&&) = delete;
    
    public:
        [[nodiscard]] Table* select(void) noexcept;
        [[nodiscard]] Table* insert(void) noexcept;
        [[nodiscard]] Table* update(void) noexcept;
        [[nodiscard]] Table* drop(void) noexcept;
    
    public:
        template<typename ...Args>
        [[nodiscard]] Table* columns(const std::pair<std::string, Args>&... columnList);
        [[nodiscard]] Table* columns(const std::initializer_list<std::string>& columnList);

    public:
        [[nodiscard]] Table* where(const std::string& condition);
        [[nodiscard]] Table* orderBy(const std::initializer_list<std::string>& columnList);
        [[nodiscard]] Table* limit(std::uint32_t limit, std::uint32_t offset = {});
    
        void rollback(void);
        [[nodiscard]] std::optional<pqxx::result> execute(void);
    
        [[nodiscard]] const std::string& getQuery() const noexcept { return m_query.str(); }
    
    private:
        enum class Operation : std::uint8_t
        {
            NOTHING,
            SELECT,
            INSERT,
            UPDATE,
            DELETE
        } m_choosedOp;

        std::ostringstream m_query;
        const std::string m_tableName;
        std::shared_ptr<PostgreAdapter> mp_postgre;

    };

    void foo() 
    {   
        using namespace std::literals::string_literals;

        Table("a").select()->columns({"*"})->where("id > 10")->limit(5, 1)->execute();
        Table("b").insert()->columns(std::pair{"'age'"s, 25},
                                     std::pair{"'sex'"s, "'m'"})->execute();
    }
}
