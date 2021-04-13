#pragma once

#include "PostgreAdapter.hpp"

#include <Utility/Exception.hpp>

namespace DataAccess
{
    class Table;

    enum class SQLStatement : std::uint8_t
    {
        UNKNOWN,
        SELECT,
        INSERT,
        UPDATE,
        DELETE
    };

    class SQLBase
    {
    public:
        SQLBase(SQLStatement statement, Table& table)
            : _statement{statement}, _currentTable{table}, _queryStream{} {}

    public:
        SQLBase() = delete;
        SQLBase(const SQLBase&) = delete;
        SQLBase& operator=(const SQLBase&) = delete;

        SQLBase(SQLBase&&) = delete;
        SQLBase& operator=(SQLBase&&) = delete;

        virtual ~SQLBase() = default;

    public:
        virtual SQLStatement type(void) const noexcept final;

    public:
        virtual void rollback(void) noexcept final;
        [[nodiscard]] virtual const std::string& getQuery() const noexcept final;
        [[nodiscard]] virtual std::optional<pqxx::result> execute(void) final;

    protected:
        std::ostringstream _queryStream;
        SQLStatement _statement;
        Table& _currentTable;

    };

    class SQLSelect : public SQLBase
    {
    public:
        SQLSelect(Table& table) : SQLBase(SQLStatement::SELECT, table) {}

        virtual ~SQLSelect() = default;
        
    public:
        SQLSelect() = delete;
        SQLSelect(const SQLSelect&) = delete;
        SQLSelect& operator=(const SQLSelect&) = delete;

        SQLSelect(SQLSelect&&) = delete;
        SQLSelect& operator=(SQLSelect&&) = delete;
    
    public:
        [[nodiscard]] SQLSelect* fields(const std::initializer_list<std::string>& columnList);

    public:
        [[nodiscard]]
        SQLSelect* where(const std::string& condition = {});
        [[nodiscard]]
        SQLSelect* limit(std::uint32_t limit, std::uint32_t offset = {});
        [[nodiscard]]
        SQLSelect* orderBy(const std::initializer_list<std::string>& columnList);
        //[[nodiscard]] SQLSelect* groupBy();
        //[[nodiscard]] SQLSelect* join(joinType);
        //[[nodiscard]] SQLSelect* having();
        //[[nodiscard]] SQLSelect* between();
        //[[nodiscard]] SQLSelect* distinct();

        [[nodiscard]]
        SQLSelect* And(const std::string& condition);
        [[nodiscard]]
        SQLSelect* Or(const std::string& condition);
        [[nodiscard]]
        SQLSelect* Not(const std::string& condition = {});
    
    private:
        friend class Table;

    };

    class SQLInsert : public SQLBase
    {
    public:
        SQLInsert(Table& table) : SQLBase(SQLStatement::INSERT, table) {}

        virtual ~SQLInsert() = default;

    public:
        SQLInsert() = delete;
        SQLInsert(const SQLInsert&) = delete;
        SQLInsert& operator=(const SQLInsert&) = delete;
    
        SQLInsert(SQLInsert&&) = delete;
        SQLInsert& operator=(SQLInsert&&) = delete;
    
    public:
        template<typename ...DataType>
        [[nodiscard]] SQLInsert* fields(const DataType&...data);
        template<typename ...DataType>
        [[nodiscard]] SQLInsert* fields(const std::tuple<DataType...>& dataList);
        template<typename ColumnType = const char* ,typename ...DataType>
        [[nodiscard]] SQLInsert* fields(const std::pair<ColumnType, DataType>& ...columnData);
        template<typename ColumnType = const char* ,typename ...DataType>
        [[nodiscard]] SQLInsert* fields(const std::tuple<std::pair<ColumnType, DataType> ...>& columnDataList);

    public:
        [[nodiscard]] SQLInsert* returning(const std::initializer_list<std::string>& columnList);

    private:
        friend class Table;
    
    };
    
    //class SQLUpdate : public SQLBase
    //{
    //public:
    //    SQLUpdate() = default;
    //    SQLUpdate(const SQLUpdate&) = default;
    //    SQLUpdate& operator=(const SQLUpdate&) = default;
    //
    //    SQLUpdate(SQLUpdate&&) = delete;
    //    SQLUpdate& operator=(SQLUpdate&&) = delete;
    //
    //    virtual ~SQLUpdate() = default;
    //
    //public:
    //    
    //
    //    [[nodiscard]] SQLSelect* columns(const std::initializer_list<std::string>& columnList);
    //
    //private:
    //
    //
    //};
    //
    //class SQLDelete : public SQLBase
    //{
    //public:
    //    SQLDelete() = default;
    //    SQLDelete(const SQLDelete&) = default;
    //    SQLDelete& operator=(const SQLDelete&) = default;
    //
    //    SQLDelete(SQLDelete&&) = delete;
    //    SQLDelete& operator=(SQLDelete&&) = delete;
    //
    //    virtual ~SQLDelete() = default;
    //
    //public:
    //
    //private:
    //
    //
    //};

    class Table
    {
    public:
        explicit Table(const char* tableName) : _tableName{tableName} 
        {
            _postgre = PostgreAdapter::getPostgre(
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
        [[nodiscard]] SQLSelect* Select(void) noexcept;
        [[nodiscard]] SQLInsert* Insert(void) noexcept;
        [[nodiscard]] SQLUpdate* Update(void) noexcept;
        [[nodiscard]] SQLDelete* Delete(void) noexcept;
    
    public:
        //template<typename ...Args>
        //[[nodiscard]] Table* columns(const std::pair<std::string, Args>&... columnList);
        //[[nodiscard]] Table* columns(const std::initializer_list<std::string>& columnList);

    public:
        void changeTable(const char* newTableName) noexcept;
        void changeTable(const std::string& newTableName) noexcept;
    
    private:
        const std::string _tableName;

        std::shared_ptr<PostgreAdapter> _postgre;

        SQLSelect* _select;
        SQLInsert* _insert;
        SQLUpdate* _update;
        SQLDelete* _delete;

    private:
        friend class SQLBase;
        friend class SQLSelect;
        friend class SQLInsert;
        friend class SQLUpdate;
        friend class SQLDelete;

    };


    





    void test() 
    {   
        using namespace std::literals::string_literals;

        Table("users").Select()->fields({"*"})->where()->Not("id = 4")->execute();
        Table("users").Insert()->fields(5, "Name")->returning({"id"})->execute();
    }
}
