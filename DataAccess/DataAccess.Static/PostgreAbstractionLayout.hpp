#pragma once

#include "PostgreAdapter.hpp"

#include <Utility/Exception.hpp>

namespace DataAccess
{
    class Table;
    class SQLBase;

    enum class SQLStatement : std::uint8_t
    {
        ST_UNKNOWN,
        ST_SELECT,
        ST_INSERT,
        ST_UPDATE,
        ST_DELETE
    };
    enum class JoinType : std::uint8_t
    {
        J_INNER,
        J_LEFT,
        J_RIGHT,
        j_FULL
    };

    template<class T>
    class SQLWhereCondition
    {
    public:

        SQLWhereCondition(T* statement) : _statement{statement} {}

        virtual ~SQLWhereCondition(void) = default;

    public:

        T* where(const std::string& condition = {})
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "where " << condition;

            return _statement;
        }
        T* And(const std::string& condition)
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "and " << condition;

            return _statement;
        }
        T* Or(const std::string& condition)
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "or " << condition;

            return _statement;
        }
        T* Not(const std::string& condition = {}) 
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "not " << condition;

            return _statement;
        }
        T* In(const std::string& anotherStatement) 
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "in (" << anotherStatement << ")";

            return _statement;
        }
        T* In(const std::initializer_list<std::string>& valueList)
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "in (";
            for (auto& value : valueList)
            {
                _statement->_queryStream << value;
                _statement->_queryStream << (value != *(valueList.end() - 1) ? ", " : "");
            }

            _statement->_queryStream << ")";

            return _statement;
        }
        template <typename ValueType>
        T* between(ValueType left, ValueType right)
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ')
                _statement->_queryStream << " ";

            _statement->_queryStream << "between " << left << " and " << right;

            return _statement;
        }

    private:
        T* _statement;
    };


    class SQLBase
    {
    public:

        SQLBase(SQLStatement statement, Table& table)
            : _statement{statement}, _currentTable{table}, _queryStream{} {}

    public:

        SQLBase(void) = delete;

        SQLBase(const SQLBase&) = delete;
        SQLBase& operator=(const SQLBase&) = delete;

        SQLBase(SQLBase&&) = delete;
        SQLBase& operator=(SQLBase&&) = delete;

        virtual ~SQLBase(void) = default;

    public:

        virtual SQLStatement type(void) const noexcept final;

    public:

        virtual const std::string getQuery() const noexcept final;
        virtual std::optional<pqxx::result> execute(void) final;

        virtual void rollback(void);
        
    protected:

        SQLStatement _statement;
        Table& _currentTable;
        std::ostringstream _queryStream;

    private:

        friend class SQLWhereCondition<SQLBase>;

    };

    class SQLSelect : public SQLBase, public SQLWhereCondition<SQLSelect>
    {
    public:

        SQLSelect(Table& table) : SQLBase(SQLStatement::ST_SELECT, table), SQLWhereCondition(this) {}

        virtual ~SQLSelect(void) = default;
        
    public:

        SQLSelect(void) = delete;

        SQLSelect(const SQLSelect&) = delete;
        SQLSelect& operator=(const SQLSelect&) = delete;

        SQLSelect(SQLSelect&&) = delete;
        SQLSelect& operator=(SQLSelect&&) = delete;
    
    public:

        SQLSelect* columns(const std::initializer_list<std::string>& columnList);

    public:

        SQLSelect* limit(std::uint32_t limit, std::uint32_t offset = {});
        SQLSelect* orderBy(const std::initializer_list<std::string>& columnList, bool desc = false);
        SQLSelect* distinct(void);
        SQLSelect* join(JoinType join, const std::string& tableName, const std::string& onCondition);
        
        SQLSelect* groupBy(const std::initializer_list<std::string>& columnList);
        SQLSelect* having(const std::string& condition);
        SQLSelect* Any(const std::string& condition);
        SQLSelect* All(const std::string& condition); 
    
    private:
        friend class Table;
        friend class SQLWhereCondition<SQLSelect>;
    };

    class SQLInsert : public SQLBase
    {
    public:

        SQLInsert(Table& table) : SQLBase(SQLStatement::ST_INSERT, table) {}

        virtual ~SQLInsert(void) = default;

    public:

        SQLInsert(void) = delete;

        SQLInsert(const SQLInsert&) = delete;
        SQLInsert& operator=(const SQLInsert&) = delete;
    
        SQLInsert(SQLInsert&&) = delete;
        SQLInsert& operator=(SQLInsert&&) = delete;
    
    public:

        template<typename ...DataType>
        SQLInsert* field(const DataType&... data)
        {
            if (_queryStream.str().find("values") == std::string::npos)
            {
                _queryStream << " values(";
            }
            else
            {
                _queryStream << ", (";
            }

            ((_queryStream << data << ", "), ...);

            this->_correctFormating();

            _queryStream << ")";

            return this;
        }
        template<typename ...DataType>
        SQLInsert* field(const std::tuple<DataType...>& dataList)
        {
            if (_queryStream.str().find("values") == std::string::npos)
            {
                _queryStream << " values(";
            }
            else
            {
                _queryStream << ", (";
            }

            std::apply(
                [this](const auto&... tupleArgs)
                { 
                    ((_queryStream << tupleArgs << ", "), ...); }
                , dataList);

            this->_correctFormating();

            _queryStream << ")";

            return this;
        }
        template<typename ColumnType = const char*, typename ...DataType>
        SQLInsert* columns(const std::pair<ColumnType, DataType>&... columnData)
        {
            _queryStream << "(";
            ((_queryStream << columnData.first << ", "), ...);

            this->_correctFormating();

            _queryStream << ")";

            _queryStream << " values(";
            ((_queryStream << columnData.second << ", "), ...);

            this->_correctFormating();

            _queryStream << ")";

            return this;
        }
        template<typename ColumnType = const char*, typename ...DataType>
        SQLInsert* columns(const std::tuple<std::pair<ColumnType, DataType>...>& columnDataList)
        {
            _queryStream << "(";
            std::apply(
                [this](const auto&... tupleArgs) 
                {
                    ((_queryStream << tupleArgs.first << ", "), ...); 
                }, columnDataList);

            this->_correctFormating();

            _queryStream << ")";

            _queryStream << " values(";
            std::apply([this](const auto&... tupleArgs) 
                {
                     ((_queryStream << tupleArgs.second << ", "), ...);
                }, columnDataList);

            this->_correctFormating();

            _queryStream << ")";

            return this;
        }

    public:

        SQLInsert* returning(const std::initializer_list<std::string>& columnList);

    private:

        void _correctFormating(void);

    private:
        friend class Table;
    };
    
    class SQLUpdate : public SQLBase, public SQLWhereCondition<SQLUpdate>
    {
    public:

        SQLUpdate(Table& table) : SQLBase(SQLStatement::ST_UPDATE, table), SQLWhereCondition(this) {}

        virtual ~SQLUpdate(void) = default;

    public:

        SQLUpdate(void) = delete;

        SQLUpdate(const SQLUpdate&) = delete;
        SQLUpdate& operator=(const SQLUpdate&) = delete;

        SQLUpdate(SQLUpdate&&) = delete;
        SQLUpdate& operator=(SQLUpdate&&) = delete;

    public:

        template<typename ColumnType = const char*, typename ...Args>
        SQLUpdate* fields(const std::pair<ColumnType, Args>&... columnData)
        {
            _queryStream << " set ";

            ((_queryStream << columnData.first << " = " << columnData.second << ", "), ...);

            this->_correctFormating();

            return this;
        }
        template <typename ColumnType = const char*, typename... Args>
        SQLUpdate* fields(const std::tuple<std::pair<ColumnType, Args> ...>& columnData)
        {
            _queryStream << " set ";

            std::apply(
                [this](const auto&... tupleArg) 
                {
                    ((_queryStream << tupleArg.first 
                                   << " = " 
                                   << tupleArg.second
                                   << ", "), ...);
                }, columnData);

            this->_correctFormating();

            return this;
        }

    private:

        void _correctFormating(void);

    private:
        friend class Table;
        friend class SQLWhereCondition<SQLUpdate>;
    };

    class SQLDelete : public SQLBase, public SQLWhereCondition<SQLDelete>
    {
    public:

        SQLDelete(Table& table) : SQLBase(SQLStatement::ST_UPDATE, table), SQLWhereCondition(this) {}

        virtual ~SQLDelete(void) = default;

    public:

        SQLDelete(void) = delete;

        SQLDelete(const SQLDelete&) = delete;
        SQLDelete& operator=(const SQLDelete&) = delete;

        SQLDelete(SQLDelete&&) = delete;
        SQLDelete& operator=(SQLDelete&&) = delete;

    private:
        friend class Table;
        friend class SQLWhereCondition<SQLDelete>;
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

        virtual ~Table(void);
    
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

        void _clear(SQLStatement statement);

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
