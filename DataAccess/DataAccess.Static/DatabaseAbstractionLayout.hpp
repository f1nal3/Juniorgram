#pragma once

#include <iostream>
#include <type_traits>
#include <variant> 

#include "PostgreAdapter.hpp"
#include "SQLCipherAdapter.hpp"

#include <Utility/Exception.hpp>
#include <Utility/Utility.hpp>

namespace DataAccess
{
    class QueryCreator;
    class SQLBase;

    /** @enum SQL statements enum.
    */
    enum class SQLStatement : std::uint8_t
    {
        ST_UNKNOWN, /// Unknown statement
        ST_SELECT, /// SELECT statement
        ST_INSERT, /// INSERT statement
        ST_UPDATE, /// UPDATE statement
        ST_DELETE  /// DELETE statement
    };
    /** @enum SQL join types enum.
    */
    enum class SQLJoinType : std::uint8_t
    {
        J_INNER, /// Standart inner sql join
        J_LEFT,  /// Left sql join
        J_RIGHT, /// Right sql join
        J_FULL   /// Full sql join
    };
    /** @enum Database type enum.
    */
    enum class DBType : std::uint8_t
    {
        DB_POSTGRE,
        DB_LITE
    };


    /** @class SQLWhereCondition.
    *   @brief SQLWhereCondition class.
    *   @details This class is a secondary. It needs for \
    *    SQLSelect, SQLUpdate and SQLUpdate. \
    *    This class using for 'where' condition from SQL
    *   @warning You shouldn't use it itself. Only from QueryCreator class.
    */
    template<class T>
    class SQLWhereCondition
    {
    public:

        SQLWhereCondition(T* statement) : _statement{statement} {}

        virtual ~SQLWhereCondition(void) = default;

    public:

        /** @brief Like common SQL 'where' condition.
        *   @params condition - SQL condition, but it is not required \
        *   @return Current SQLSTATEMENT pointer object to continue SQL query.
        */
        T* Where(const std::string& condition = {})
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "where " << condition;

            return _statement;
        }
        /** @brief Like common SQL 'and' condition. This condition \
        *    need for continue the SQL query after like: limit, beetween, etc.
        *   @params condition - SQL condition, but it is not required. \
        *   @return Current SQLSTATEMENT pointer object to continue SQL query.
        */
        T* And(const std::string& condition = {})
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ')
                _statement->_queryStream << " ";

            _statement->_queryStream << "and " << condition;

            return _statement;
        }
        /** @brief Like common SQL 'or' condition. This condition \
        *    need for continue the SQL query after like: limit, beetween, etc.
        *   @params condition - SQL condition, but it is not required. \
        *   @return Current SQLSTATEMENT pointer object to continue SQL query.
        */
        T* Or(const std::string& condition = {})
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ')
                _statement->_queryStream << " ";

            _statement->_queryStream << "or " << condition;

            return _statement;
        }
        /** @brief In condition.
         *  @params anotherStatement - another SQL statement, \
         *   that you can receive from 'getQuery()' method.
         *  @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
        T* In(const std::string& anotherStatement) 
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ') 
                _statement->_queryStream << " ";

            _statement->_queryStream << "in (" << anotherStatement << ")";

            return _statement;
        }
        /** @brief In condition.
         *   @params valueList - checking from list of values.
         *   @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
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
        /** @brief Beeween condition.
         *  @params left - start of the range.
         *   right - end of the range.
         *  @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
        template <typename ValueType>
        T* Between(ValueType left, ValueType right)
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ')
                _statement->_queryStream << " ";

            _statement->_queryStream << "between " << left << " and " << right;

            return _statement;
        }
        /** @brief Like condition.
        *   @params pattern - see this for info: \
        *    https://www.w3schools.com/sql/sql_like.asp, \
        *    https://www.w3schools.com/sql/sql_wildcards.asp
        *   @return Current SQLSTATEMENT pointer object to continue SQL query.
        */
        T* Like(const std::string& pattern)
        {
            if (*(_statement->_queryStream.str().end() - 1) != ' ')
                _statement->_queryStream << " ";

            _statement->_queryStream << "like " << pattern;

            return _statement;
        }

    private:
        T* _statement;
    };


    /** @class SQLBase.
    *   @brief SQLBase abstraction class.
    *   @details This class has 4 non-overloaded \
    *    inherited methods.
    *   @warning Like a SQLWhereCondition \
    *   you shouldn't use it itself.
    */
    class SQLBase
    {
    public:

        SQLBase(SQLStatement statement, QueryCreator& table)
            : _statement{statement}, _currentCreator{table}, _queryStream{} {}

    public:

        SQLBase(void) = delete;

        SQLBase(const SQLBase&) = delete;
        SQLBase& operator=(const SQLBase&) = delete;

        SQLBase(SQLBase&&) = delete;
        SQLBase& operator=(SQLBase&&) = delete;

        virtual ~SQLBase(void) = default;

    public:

        /** @brief Return SQL statement type.
        *   @return ST_SELECT, ST_INSERT, ST_UPDATE, ST_DELETE
        */
        virtual SQLStatement type(void) const noexcept final;

    public:

        /** @brief Method that returns current SQL query string. \
        *    This method for subqueries basically.
        *   @return SQL query string
        *   @warning Don't clear SQL string if you use it. \
        *    After getQuery you whatever can call method execute.
        *   @code
        *    QueryCreator table("tableName");
        *    table.'SQLSTATEMENT'()->getQuery();
        *    table.'SQLSTATEMENT'()->execute(); /// Here SQL query string will be clear.
        *   @encode
        */
        virtual const std::string getQuery(void) const noexcept final;
        /** @brief Method that executes SQL string. 
        *    For technical reasons, 
             you must specify the type of container 
             where you want to put the result: 
             pqxx::result or SQLiteResultType.
             Maybe it can be fixed in the near future.
        *   @details This method clear SQL query string after call it.
        *   @return Optional object pqxx::result or SQLiteResultObject
        *    For pqxx::result check here: 
        *    https://libpqxx.readthedocs.io/en/6.4/a01127.html
        */
        virtual std::variant<std::optional<pqxx::result>> execute(void);
        

        /** @brief Method that clear SQL query string.
        *   @code
        *    QueryCreator table("tableName");
        *    table.'SQLSTATEMENT'()->qetQuery();
        *    table.'SQLSTATEMENT'()->rollback();
        *   @endcode
        */
        virtual void rollback(void) final;
        
    protected:

        SQLStatement _statement;
        QueryCreator& _currentCreator;
        std::ostringstream _queryStream;

    private:

        friend class SQLWhereCondition<SQLBase>;

    };

    /** @class SQLSelect.
    *   @brief SQLSelect class.
    *   @details It likes common SQL select condition.
    *   @warning After call it you must call the method 'columns' \
    *    (BUT distinct is exception) \
    *    and give it columns that you need to return. \
    *    OR you can give '*'.
    */
    class SQLSelect : public SQLBase, public SQLWhereCondition<SQLSelect>
    {
    public:

        SQLSelect(QueryCreator& table) : SQLBase(SQLStatement::ST_SELECT, table), SQLWhereCondition(this) {}

        virtual ~SQLSelect(void) = default;
        
    public:

        SQLSelect(void) = delete;

        SQLSelect(const SQLSelect&) = delete;
        SQLSelect& operator=(const SQLSelect&) = delete;

        SQLSelect(SQLSelect&&) = delete;
        SQLSelect& operator=(SQLSelect&&) = delete;
    
    public:

        /** @brief Columns that you need to return from database.
        *   @details You must point columns that you need, like:
        *   @code 
        *    ...->columns({"column1", "column2", ...})->...;
        *   @endcode
        *   @details Or, for all columns from table use:
        *   @code
        *    ...->columns({"*"})->...;
        *   @params Inintializer_list<string> of columns that you need.
        *   @return Current SQLSelect pointer object to continue SQL query.
        */
        SQLSelect* columns(const std::initializer_list<std::string>& columnList);

    public:

        /** @brief Method for limiting returned SQL rows.
        *   @details For example limit 5 offset 2 return: \
        *    row3, row4, row5, row6, row7.
        *   @params limit - how much rows that you need
        *    offset - from what row start limit
        *   @return Current SQLSelect pointer object to continue SQL query.
        */
        SQLSelect* limit(std::uint32_t limit, std::uint32_t offset = {});
        /** @brief Method for selecting rows by columns in alphabetic order.
        *   @details Use desc = true, if you wanna select in reverse order.  
        *   @params columnList - list of columns \
        *    desc - reverse order
        *   @code 
        *    ...->orderBy({"column1", "column2", ...})->...;
        *    ...->orderBy({"*"})->...;
        *   @endcode
        *   @retunrn Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* orderBy(const std::initializer_list<std::string>& columnList, bool desc = false);
        /** @brief Method that select only distinct(different) values.
        *   @warning You must use it only before columns method!
        *   @return Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* distinct(void);
        /** @brief Method that select join tables.
        *   @params join - join type (Check SQLJoinType enum) \
        *    secondTableName - table with which will be join select \
        *    onCondition - important condition for join tables
        *   @return Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* join(SQLJoinType join, const std::string& secondTableName, const std::string& onCondition);
        
        /** @brief Method for group result table.
        *   @params columnList - list of the columns. \
        *    Use ...({"*"})->... for all columns
        *   @return Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* groupBy(const std::initializer_list<std::string>& columnList);
        /** @brief Method for using instead of where with aggregate functions.
        *   @code 
        *    SELECT Employees.LastName, COUNT(Orders.OrderID) AS NumberOfOrders
             FROM Orders
             INNER JOIN Employees ON Orders.EmployeeID = Employees.EmployeeID
             WHERE LastName = 'Davolio' OR LastName = 'Fuller'
             GROUP BY LastName
             HAVING COUNT(Orders.OrderID) > 25;
            @endcode
        *   @param condition - string SQL condition
        *   @return Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* having(const std::string& condition);
        /** @brief Method for a condition between a single column \
        *    and a range of other values (any of the values).
        *   @details It's equivolent of: \
        *    x = ANY (a,b,c) -> x = a OR b OR c
        *   @params SQL subQuery string
        *   @return Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* Any(const std::string& subQuery);
        /** @brief Method for a condition between a single column \
        *    and a range of other values (a of the values).
        *   @params SQL subQuery string
        *   @return Current SQLSelect pointer object to continue SQL query
        */
        SQLSelect* All(const std::string& subQuery);
    
    private:
        friend class QueryCreator;
        friend class SQLWhereCondition<SQLSelect>;
    };

    /** @class SQLInsert.
    *   @brief SQLInsert class.
    *   @details It likes common SQL insert condition.
    */
    class SQLInsert : public SQLBase
    {
    public:

        SQLInsert(QueryCreator& table) : SQLBase(SQLStatement::ST_INSERT, table) {}

        virtual ~SQLInsert(void) = default;

    public:

        SQLInsert(void) = delete;

        SQLInsert(const SQLInsert&) = delete;
        SQLInsert& operator=(const SQLInsert&) = delete;
    
        SQLInsert(SQLInsert&&) = delete;
        SQLInsert& operator=(SQLInsert&&) = delete;
    
    public:

        /** @brief Insert one row into table.
        *   @details Inserting all columns into a row.
        *   @code
        *    ...->field(1, "a", "male")->field(2, "b", "female")...;
        *   @endcode
        *   @params list of row's columns.
        *   @return Current SQLInsert pointer object to continue SQL query.
        */
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

            ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(data) << ", "), ...);

            this->privateCorrectFormating();

            _queryStream << ")";

            return this;
        }
        /** @brief Insert one row into table.
         *  @details Inserting columns into a row as tuple. \
         *  @code
         *   ...->field(tupleName1)->field(tupleName2)...;
         *  @endcode
         *  @params Taple with data.
         *  @return Current SQLInsert pointer object to continue SQL query.
         */
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
                    ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(tupleArgs) << ", "), ...);
                }
                , dataList);

            this->privateCorrectFormating();

            _queryStream << ")";

            return this;
        }
        /** @brief Insert rows into table.
         *  @details Inserting several columns into a row as pairs. \
         *   You don't have to wrap columnName by quotes, \
         *   BUT you must wrap data strings.
         *  @code
         *   ...->columns(pair{"Column1", 1}, pair{"Column2", "data"})->field(data)->...;
         *  @endcode
         *  @params Pairs.
         *  @return Current SQLInsert pointer object to continue SQL query.
         */
        template<typename ColumnType = const char*, typename ...DataType>
        SQLInsert* columns(const std::pair<ColumnType, DataType>&... columnData)
        {
            _queryStream << "(";
            ((_queryStream << columnData.first << ", "), ...);

            this->privateCorrectFormating();

            _queryStream << ")";

            _queryStream << " values(";
            ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(columnData.second) << ", "), ...);

            this->privateCorrectFormating();

            _queryStream << ")";

            return this;
        }
        /** @brief Insert rows into table.
         *  @details Inserting several columns into a row as tuple of pairs. \
         *   You don't have to wrap columnName by quotes. \
         *  @code
         *   ...->columns(tupleName1)->field(data)->...;
         *  @endcode
         *  @params Tuple of pairs.
         *  @return Current SQLInsert pointer object to continue SQL query.
         */
        template<typename ColumnType = const char*, typename ...DataType>
        SQLInsert* columns(const std::tuple<std::pair<ColumnType, DataType>...>& columnDataList)
        {
            _queryStream << "(";
            std::apply(
                [this](const auto&... tupleArgs) 
                {
                    ((_queryStream << tupleArgs.first << ", "), ...); 
                }, columnDataList);

            this->privateCorrectFormating();

            _queryStream << ")";

            _queryStream << " values(";
            std::apply([this](const auto&... tupleArgs) 
                {
                     ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(tupleArgs.second) << ", "), ...);
                }, columnDataList);

            this->privateCorrectFormating();

            _queryStream << ")";

            return this;
        }

    public:

        /** @brief Returning columns from table after insert.
        *   @params columnList - list of columns that need to return.
        *   @return Current SQLInsert pointer object to continue SQL query.
        */
        SQLInsert* returning(const std::initializer_list<std::string>& columnList);

    private:

        void privateCorrectFormating(void);

    private:
        friend class QueryCreator;
    };
    
    /** @class SQLUpdate.
    *   @brief SQLUpdate class.
    *   @details It likes common SQL update condition.
    */
    class SQLUpdate : public SQLBase, public SQLWhereCondition<SQLUpdate>
    {
    public:

        SQLUpdate(QueryCreator& table) : SQLBase(SQLStatement::ST_UPDATE, table), SQLWhereCondition(this) {}

        virtual ~SQLUpdate(void) = default;

    public:

        SQLUpdate(void) = delete;

        SQLUpdate(const SQLUpdate&) = delete;
        SQLUpdate& operator=(const SQLUpdate&) = delete;

        SQLUpdate(SQLUpdate&&) = delete;
        SQLUpdate& operator=(SQLUpdate&&) = delete;

    public:

        /** @brief Updating fields with new data.
        *   @params columnData - pairs with column names by first \
        *    and the data by second.
        *   @return Current SQLUpdate pointer object to continue SQL query.
        */
        template<typename ColumnType = const char*, typename ...Args>
        SQLUpdate* fields(const std::pair<ColumnType, Args>&... columnData)
        {
            _queryStream << " set ";

            ((_queryStream << columnData.first 
                           << " = " 
                           << Utility::CheckForSQLSingleQuotesProblem(columnData.second)
                           << ", "), ...);

            this->privateCorrectFormating();

            return this;
        }
        /** @brief Updating fields with new data.
        *   @params columnData - tuple of pairs.
        *   @return Current SQLUpdate pointer object to continue SQL query.
        */
        template <typename ColumnType = const char*, typename... Args>
        SQLUpdate* fields(const std::tuple<std::pair<ColumnType, Args> ...>& columnData)
        {
            _queryStream << " set ";

            std::apply(
                [this](const auto&... tupleArg) 
                {
                    ((_queryStream << tupleArg.first 
                                   << " = " 
                                   << Utility::CheckForSQLSingleQuotesProblem(tupleArg.second)
                                   << ", "), ...);
                }, columnData);

            this->privateCorrectFormating();

            return this;
        }

    private:

        void privateCorrectFormating(void);

    private:
        friend class QueryCreator;
        friend class SQLWhereCondition<SQLUpdate>;
    };

    /** @class SQLDelete.
    *   @brief SQLDelete class.
    *   @details It likes common SQL delete condition.
    */
    class SQLDelete : public SQLBase, public SQLWhereCondition<SQLDelete>
    {
    public:

        SQLDelete(QueryCreator& table) : SQLBase(SQLStatement::ST_UPDATE, table), SQLWhereCondition(this) {}

        virtual ~SQLDelete(void) = default;

    public:

        SQLDelete(void) = delete;

        SQLDelete(const SQLDelete&) = delete;
        SQLDelete& operator=(const SQLDelete&) = delete;

        SQLDelete(SQLDelete&&) = delete;
        SQLDelete& operator=(SQLDelete&&) = delete;

    private:
        friend class QueryCreator;
        friend class SQLWhereCondition<SQLDelete>;
    };

    /* @class QueryCreator.
    *  @brief QueryCreator class.
    *  @details You can see some examples below for how to use it.
    *  @code
    *   QueryCreator(*DBTypeEnum*, "tableName1").Select()->columns({"column1", "column2", ...})->where("condition")->...->execute()/OR/getQuery();
    *   QueryCreator(*DBTypeEnum*, "tableName2").Insert()->field(1, "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
    *   QueryCreator(*DBTypeEnum*, "tableName3").Update()->fields(pair{"column1", 1}, pair{"column2", "strData"})->where("condition")->...->execute()/OR/getQuery();
    *   QueryCreator(*DBTypeEnum*, "tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();  
    *  @endcodegit 
    */
    class QueryCreator
    {
    public:

        explicit QueryCreator(DBType type, const char* tableName)
            : _databaseType{type},
              _tableName{tableName},
              _select{nullptr},
              _insert{nullptr},
              _update{nullptr},
              _delete{nullptr} 
        {
            switch (type)
            {
            case DBType::DB_LITE:
            {
                _adapter = SQLCipherAdapter::getInstance<SQLCipherAdapter>();
            }
            break;
            case DBType::DB_POSTGRE:
            {
                _adapter = PostgreAdapter::getInstance<PostgreAdapter>();
            }
            break;
            default:
                break;
            }
        }
        explicit QueryCreator(DBType type, const std::string& tableName)
            : QueryCreator(type, tableName.c_str()) {}
        explicit QueryCreator(DBType type, const std::string_view& tableName)
            : QueryCreator(type, tableName.data()) {}
        explicit QueryCreator(DBType type, const char* tableName, const std::string_view& options)
            : _databaseType{type},
              _tableName{tableName},
              _select{nullptr},
              _insert{nullptr},
              _update{nullptr},
              _delete{nullptr}
        {
            switch (type)
            {
            case DBType::DB_LITE:
            {
                _adapter = SQLCipherAdapter::getInstance<SQLCipherAdapter>(options);
            }
            break;
            case DBType::DB_POSTGRE:
            {
                _adapter = PostgreAdapter::getInstance<PostgreAdapter>(options);
            }
            break;
            default:
                break;
            }
        }
        explicit QueryCreator(DBType type, const std::string& tableName, const std::string_view& options)
            : QueryCreator(type, tableName.c_str(), options) {}
        explicit QueryCreator(DBType type, const std::string_view& tableName, const std::string_view& options)
            : QueryCreator(type, tableName.data(), options) {}

        virtual ~QueryCreator(void);
    
    public:

        QueryCreator() = delete;
    
        QueryCreator(const QueryCreator&) = delete;
        QueryCreator(QueryCreator&&)      = delete;
    
        QueryCreator& operator=(const QueryCreator&) = delete;
        QueryCreator& operator=(QueryCreator&&) = delete;
    
    public:

        /** @brief SQL select query.
        *   @return SQLSelect pointer.
        */
        SQLSelect* Select(void);
        /** @brief SQL insert query.
        *   @return SQLInsert pointer.
        */
        SQLInsert* Insert(void);
        /** @brief SQL update query.
        *   @return SQLUpdate pointer.
        */
        SQLUpdate* Update(void);
        /** @brief SQL delete query.
        *   @return SQLDelete pointer.
        */
        SQLDelete* Delete(void);
    
    public:

        /** @brief Changing table.
        *   @params newTableName - new name of the table.
        */
        void changeTable(const char* newTableName) noexcept;
        /** @brief Changing table.
         *   @params newTableName - new name of the table.
         */
        void changeTable(const std::string& newTableName) noexcept;
    
        /** @brief Get postgreAdapter object.
        *   @return PostgreAdapter.
        */
        std::shared_ptr<IAdapter> getAdapter(void) const noexcept;

    private:

        void privateClear(SQLStatement statement);

    private:

        DBType _databaseType;

        std::string _tableName;

        std::shared_ptr<IAdapter> _adapter;

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

    /** @class Postgre table.
    *   @brief Postgre table class.
    *   @details You can see some examples below for how to use it.
    *   @code
    *    PTable("tableName1").Select()->columns({"column1", "column2", ...})->where("condition")->...->execute()/OR/getQuery();
    *    PTable("tableName2").Insert()->field(1, "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
    *    PTable("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2", "strData"})->where("condition")->...->execute()/OR/getQuery();
    *    PTable("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();  
    *   @endcodegit 
    */
    class PTable : public QueryCreator
    {
    public:

        explicit PTable(const char* tableName) 
            : QueryCreator(DBType::DB_POSTGRE, tableName) {}
        explicit PTable(const std::string& tableName) 
            : QueryCreator(DBType::DB_POSTGRE, tableName) {}
        explicit PTable(const std::string_view& tableName) 
            : QueryCreator(DBType::DB_POSTGRE, tableName) {}

        explicit PTable(const char* tableName, const std::string_view& options)
            : QueryCreator(DBType::DB_POSTGRE, tableName, options) {}
        explicit PTable(const std::string& tableName, const std::string_view& options)
            : QueryCreator(DBType::DB_POSTGRE, tableName, options) {}
        explicit PTable(const std::string_view& tableName, const std::string_view& options)
            : QueryCreator(DBType::DB_POSTGRE, tableName, options) {}
    };

    /** @class SQLite table.
    *   @brief SQLite table class.
    *   @details You can see some examples below for how to use it.
    *   @code
    *    LTable("tableName1").Select()->columns({"column1", "column2", ...})->where("condition")->...->execute()/OR/getQuery();
    *    LTable("tableName2").Insert()->field(1, "a")->field(...)->...->returning({"column1", "column2", ...})->execute()/OR/getQuery();
    *    LTable("tableName3").Update()->fields(pair{"column1", 1}, pair{"column2", "strData"})->where("condition")->...->execute()/OR/getQuery();
    *    LTable("tableName4").Delete()->where("condition")->...->execute()/OR/getQuery();  
    *   @endcodegit 
    */
    class LTable : public QueryCreator
    {
    public:

        explicit LTable(const char* tableName)
            : QueryCreator(DBType::DB_LITE, tableName) {}
        explicit LTable(const std::string& tableName)
            : QueryCreator(DBType::DB_LITE, tableName) {}
        explicit LTable(const std::string_view& tableName)
            : QueryCreator(DBType::DB_LITE, tableName) {}

        explicit LTable(const char* tableName, const std::string_view& options)
            : QueryCreator(DBType::DB_LITE, tableName, options) {}
        explicit LTable(const std::string& tableName, const std::string_view& options)
            : QueryCreator(DBType::DB_LITE, tableName, options) {}
        explicit LTable(const std::string_view& tableName, const std::string_view& options)
            : QueryCreator(DBType::DB_LITE, tableName, options) {}
    };
}
