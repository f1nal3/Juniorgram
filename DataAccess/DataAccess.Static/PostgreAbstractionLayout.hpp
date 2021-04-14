#pragma once

#include "PostgreAdapter.hpp"

#include <Utility/Exception.hpp>

namespace DataAccess
{
    class Table;

    enum class SQLStatement : std::uint8_t
    {
        ST_UNKNOWN,
        ST_SELECT,
        ST_INSERT,
        ST_UPDATE,
        ST_DELETE
    };


    class SQLBase
    {
    public:

        SQLBase(SQLStatement statement, Table& table)
            : _statement{statement}, _currentTable{table}, _queryStream{} {}

    public:

        SQLBase(void) = delete;

        SQLBase(const SQLBase&) = default;
        SQLBase& operator=(const SQLBase&) = default;

        SQLBase(SQLBase&&) = delete;
        SQLBase& operator=(SQLBase&&) = delete;

        virtual ~SQLBase(void) = default;

    public:

        virtual SQLStatement type(void) const noexcept final;

    public:

        virtual const std::string getQuery() const noexcept final;
        virtual std::optional<pqxx::result> execute(void) final;

        virtual void rollback(void){}

    protected:

        std::ostringstream _queryStream;
        SQLStatement _statement;
        Table& _currentTable;
    };

    class SQLSelect : public SQLBase
    {
    public:

        SQLSelect(Table& table) : SQLBase(SQLStatement::ST_SELECT, table) {}

        virtual ~SQLSelect(void) = default;
        
    public:

        SQLSelect(void) = delete;

        SQLSelect(const SQLSelect&) = default;
        SQLSelect& operator=(const SQLSelect&) = default;

        SQLSelect(SQLSelect&&) = delete;
        SQLSelect& operator=(SQLSelect&&) = delete;
    
    public:

        void rollback(void) override;

        SQLSelect* fields(const std::initializer_list<std::string>& columnList);

    public:

        SQLSelect* where(const std::string& condition = {});
        SQLSelect* limit(std::uint32_t limit, std::uint32_t offset = {});
        SQLSelect* orderBy(const std::initializer_list<std::string>& columnList, bool desc = false);
        //[[nodiscard]] SQLSelect* groupBy();
        //[[nodiscard]] SQLSelect* join(joinType);
        //[[nodiscard]] SQLSelect* having();
        //[[nodiscard]] SQLSelect* between();
        //[[nodiscard]] SQLSelect* distinct();

        SQLSelect* And(const std::string& condition);
        SQLSelect* Or(const std::string& condition);
        SQLSelect* Not(const std::string& condition = {});
    
    private:

        friend class Table;
    };

    class SQLInsert : public SQLBase
    {
    public:

        SQLInsert(Table& table) : SQLBase(SQLStatement::ST_INSERT, table) {}

        virtual ~SQLInsert(void) = default;

    public:

        SQLInsert(void) = delete;

        SQLInsert(const SQLInsert&) = default;
        SQLInsert& operator=(const SQLInsert&) = default;
    
        SQLInsert(SQLInsert&&) = delete;
        SQLInsert& operator=(SQLInsert&&) = delete;
    
    public:

        void rollback(void) override;

        template<typename ...DataType>
        SQLInsert* fields(const DataType&... data);
        template<typename ...DataType>
        SQLInsert* fields(const std::tuple<DataType...>& dataList);
        template<typename ColumnType = const char* ,typename ...DataType>
        SQLInsert* fields(const std::pair<ColumnType, DataType>&... columnData);
        template<typename ColumnType = const char* ,typename ...DataType>
        SQLInsert* fields(const std::tuple<std::pair<ColumnType, DataType>...>& columnDataList);

    public:

        SQLInsert* returning(const std::initializer_list<std::string>& columnList);

    private:
        friend class Table;
    };
    
    class SQLUpdate : public SQLBase
    {
    public:

        SQLUpdate(Table& table) : SQLBase(SQLStatement::ST_UPDATE, table) {}

        virtual ~SQLUpdate(void) {}

    public:

        SQLUpdate(void) = delete;

        SQLUpdate(const SQLUpdate&) = default;
        SQLUpdate& operator=(const SQLUpdate&) = default;

        SQLUpdate(SQLUpdate&&) = delete;
        SQLUpdate& operator=(SQLUpdate&&) = delete;

    public:

        template<typename ColumnType = const char*, typename ...Args>
        SQLUpdate* field(const std::pair<ColumnType, Args>&... columnData);
        template <typename ColumnType = const char*, typename... Args>
        SQLUpdate* field(std::tuple<std::pair<ColumnType, Args> ...> columnData);

        void rollback(void) override;

    public:

         SQLUpdate* where(const std::string& condition = {});

         SQLUpdate* And(const std::string& condition);
         SQLUpdate* Or(const std::string& condition);
         SQLUpdate* Not(const std::string& condition = {});

    private:
        friend class Table;
    };

    class SQLDelete : public SQLBase
    {
    public:

        SQLDelete(Table& table) : SQLBase(SQLStatement::ST_UPDATE, table) {}

        virtual ~SQLDelete(void) {}

    public:

        SQLDelete(void) = delete;

        SQLDelete(const SQLDelete&) = default;
        SQLDelete& operator=(const SQLDelete&) = default;

        SQLDelete(SQLDelete&&) = delete;
        SQLDelete& operator=(SQLDelete&&) = delete;

    public:

        void rollback(void) override;

    public:



    private:
        friend class Table;
    };


    class Table
    {
    public:

        explicit Table(const char* tableName)
            : _tableName{tableName},
              _select{nullptr},
              _insert{nullptr},
              _update{nullptr},
              _delete{nullptr} 
        {
            _postgre = PostgreAdapter::getPostgre(
                "dbname=postgres user=postgres password=Asw789654 hostaddr=127.0.0.1 port=5432");
        }
        Table(const std::string& tableName)
            : Table(tableName.c_str()) {}

        virtual ~Table();
    
    public:

        Table() = delete;
    
        Table(const Table&) = delete;
        Table(Table&&)      = delete;
    
        Table& operator=(const Table&) = delete;
        Table& operator=(Table&&) = delete;
    
    public:

        SQLSelect* Select(void);
        SQLInsert* Insert(void);
        SQLUpdate* Update(void);
        SQLDelete* Delete(void);
    
    public:

        void changeTable(const char* newTableName) noexcept;
        void changeTable(const std::string& newTableName) noexcept;
    
    private:

        void _clearS(void)
        {
            delete _select;
            _select = nullptr;
        }
        void _clearI(void)
        {
            delete _insert;
            _insert = nullptr;
        }
        void _clearU(void)
        {
            delete _update;
            _update = nullptr;
        }
        void _clearD(void)
        {
            delete _delete;
            _delete = nullptr;
        }

    private:

        std::string _tableName;

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
}
