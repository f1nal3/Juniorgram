#pragma once

#include <tuple>
#include <vector>
#include <memory>
#include <sstream>
#include <optional>
#include <stdexcept>

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
        Table* select(void);
        Table* insert(void);
        Table* update(void);
        Table* drop(void);
    
    public:
        template<typename ...Args>
        Table* columns(const std::pair<std::string, Args>& ...columnList);
        Table* columns(const std::initializer_list<std::string>& columnList);

    public:
        Table* where(const std::string& condition);
        Table* orderBy(const std::initializer_list<std::string>& columnList);
        Table* limit(std::uint32_t limit, std::uint32_t offset = {});
    
        void rollback(void);
        std::optional<pqxx::result> execute(void);
    
        const std::string& getQuery() const noexcept { return m_query.str(); }
    
    private:
        enum class Operation : std::uint8_t
        {
            NOTHING,
            SELECT,
            INSERT,
            UPDATE,
            DELETE
        } m_choosedOp;

        std::stringstream m_query;
        std::string m_tableName;
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
