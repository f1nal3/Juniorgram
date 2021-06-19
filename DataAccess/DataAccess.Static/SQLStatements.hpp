#pragma once
#include <optional>
#include <any>
#include <iostream>

#include <Utility/SQLUtility.hpp>

namespace DataAccess
{
    template <typename ResultType>
    class QueryBuilder;

    /** @class SQLWhereCondition.
     *   @brief SQLWhereCondition class.
     *   @details This class is a secondary. It needs for \
     *    SQLSelect, SQLUpdate and SQLUpdate. \
     *    This class using for 'where' condition from SQL
     *   @warning You shouldn't use it itself. Only from QueryCreator class.
     */
    template <class T>
    class SQLWhereCondition
    {
    private:
        T* _statement;

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
            privateStatementCorrectness();

            *_statement << "where " << condition;

            return _statement;
        }
        /** @brief Like common SQL 'and' condition. This condition \
         *    need for continue the SQL query after like: limit, beetween, etc.
         *   @params condition - SQL condition, but it is not required. \
         *   @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
        T* And(const std::string& condition = {})
        {
            privateStatementCorrectness();

            *_statement << "and " << condition;

            return _statement;
        }
        /** @brief Like common SQL 'or' condition. This condition \
         *    need for continue the SQL query after like: limit, beetween, etc.
         *   @params condition - SQL condition, but it is not required. \
         *   @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
        T* Or(const std::string& condition = {})
        {
            privateStatementCorrectness();

            *_statement << "or " << condition;

            return _statement;
        }
        /** @brief In condition.
         *  @params anotherStatement - another SQL statement, \
         *   that you can receive from 'getQuery()' method.
         *  @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
        T* In(const std::string& anotherStatement)
        {
            privateStatementCorrectness();

            *_statement << "in (" << anotherStatement << ")";

            return _statement;
        }
        /** @brief In condition.
         *   @params valueList - checking from list of values.
         *   @return Current SQLSTATEMENT pointer object to continue SQL query.
         */
        T* In(const std::initializer_list<std::string>& valueList)
        {
            privateStatementCorrectness();

            *_statement << "in (";
            for (auto& value : valueList)
            {
                *_statement << value;
                *_statement << (value != *(valueList.end() - 1) ? ", " : "");
            }

            *_statement << ")";

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
            privateStatementCorrectness();

            *_statement << "between " << left << " and " << right;

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
            privateStatementCorrectness();

            *_statement << "like " << pattern;

            return _statement;
        }

    private:
        void privateStatementCorrectness(void)
        {
            if (*(_statement->getStringQuery().end() - 1) != ' ') *_statement << " ";
        }
    };

    /** @class SQLBase.
     *   @brief SQLBase abstraction class.
     *   @details This class has 4 non-overloaded inherited methods.
     *   @warning Like a SQLWhereCondition 
     *   you shouldn't use it itself.
     */
    template <typename ResultType>
    class SQLBase
    {
    protected:

        Utility::SQLStatement _statementType;
        QueryBuilder<ResultType>& _currentBuilder;
        std::ostringstream _queryStream;

    public:
        SQLBase(Utility::SQLStatement statement, QueryBuilder<ResultType>& table)
            : _statementType{statement}, _currentBuilder{table}, _queryStream{}
        {
        }

    public:
        SQLBase(void) = delete;

        SQLBase(const SQLBase&) = delete;
        SQLBase& operator=(const SQLBase&) = delete;

        SQLBase(SQLBase&&) = delete;
        SQLBase& operator=(SQLBase&&) = delete;

        virtual ~SQLBase(void) = default;

    public:

        template <typename T>
        SQLBase& operator << (T&& data)
        {
            _queryStream << std::forward<T>(data);

            return *this;
        }

    public:
        /** @brief Return SQL statement type.
         *   @return ST_SELECT, ST_INSERT, ST_UPDATE, ST_DELETE
         */
        virtual Utility::SQLStatement getStatementType(void) const noexcept final
        {
            return _statementType;
        }
        /** @brief Method that returns current SQL query string. 
         *    This method for subqueries basically.
         *   @return SQL query string
         *   @warning Don't clear SQL string if you use it. 
         *    After getQuery you whatever can call method execute.
         *   @code
         *    QueryBuilder table("tableName");
         *    table.'SQLSTATEMENT'()->getQuery();
         *    table.'SQLSTATEMENT'()->execute(); /// Here SQL query string will be clear.
         *   @encode
         */
        virtual const std::string getStringQuery(void) const noexcept final
        {
            return _queryStream.str();
        }
        /** @brief Method that executes SQL string.
        *    For technical reasons,
             you must specify the type of container
             where you want to put the result:
             pqxx::result or SQLiteResultType.
             Maybe it can be fixed in the near future.
            @warning Be careful with timestamps. That can only be c_str!!!
        *   @details This method clear SQL query string after call it.
        *   @return Optional object pqxx::result or SQLiteResultObject
        *    For pqxx::result check here:
        *    https://libpqxx.readthedocs.io/en/6.4/a01127.html
        */
        std::optional<ResultType> execute(void) 
        {
            try
            {
                if (_currentBuilder.getAdapter()->isConnected())
                {
                    std::optional<std::any> result =
                        _currentBuilder.getAdapter()->query(_queryStream.str());
        
                    if (result.has_value())
                    {
                        this->rollback();

                        return {std::any_cast<ResultType>(result.value())};
                    }
                }
                else
                {
                    throw Utility::OperationDBException("Database connection failure!", __FILE__,
                                                        __LINE__);
                }
            }
            catch (const pqxx::sql_error& err)
            {
                std::cerr << err.what() << '\n';
                std::cerr << err.query() << '\n';
            }
            catch (const std::exception& err)
            {
                std::cerr << err.what() << '\n';
            }
        
            this->rollback();
        
            return std::nullopt;
        }
        /** @brief Method that clear SQL query string.
         *   @code
         *    QueryBuilder table("tableName");
         *    table.'SQLSTATEMENT'()->qetQuery();
         *    table.'SQLSTATEMENT'()->rollback();
         *   @endcode
         */
        virtual void rollback(void) final
        { 
            _currentBuilder.clearStatement();
        }

    protected:

        void privateCheckForLastSymbol(void)
        {
            if (*(_queryStream.str().end() - 1) != ' ')
                _queryStream << " ";
        }

        void privateCorrectFormating(void)
        {
            std::string queryBuffer = _queryStream.str();
            queryBuffer.erase(queryBuffer.end() - 2, queryBuffer.end());

            _queryStream.str(queryBuffer);
            _queryStream.seekp(0, std::ios_base::end);
        }
    };
    
    /** @class SQLSelect.
     *   @brief SQLSelect class.
     *   @details It likes common SQL select condition.
     *   @warning After call it you must call the method 'columns'
     *    (BUT distinct is exception)
     *    and give it columns that you need to return.
     *    OR you can give '*'.
     */
    template <typename ResultType>
    class SQLSelect : public SQLBase<ResultType>, public SQLWhereCondition<SQLSelect<ResultType>>
    {
    public:
        SQLSelect(QueryBuilder<ResultType>& table)
            : SQLBase<ResultType>(Utility::SQLStatement::ST_SELECT, table),
              SQLWhereCondition<SQLSelect<ResultType>>(this)
        {
        }
    
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
        SQLSelect* columns(const std::initializer_list<std::string>& columnList)
        {
            for (auto& column : columnList)
            {
                _queryStream << column;
                _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
            }
            _queryStream << " from " << _currentBuilder.getCurrentTableName();

            return this;
        }
    
    public:
        /** @brief Method for limiting returned SQL rows.
         *   @details For example limit 5 offset 2 return: \
         *    row3, row4, row5, row6, row7.
         *   @params limit - how much rows that you need
         *    offset - from what row start limit
         *   @return Current SQLSelect pointer object to continue SQL query.
         */
        SQLSelect* limit(std::uint32_t limit, std::uint32_t offset = {})
        {
            privateCheckForLastSymbol();

            _queryStream << "limit " << limit << " offset " << offset;

            return this;
        }
        /** @brief Method for selecting rows by columns in alphabetic order.
         *   @details Use desc = true, if you wanna select in reverse order.
         *   @params columnList - list of columns.
         *   @params desc - reverse order.
         *   @code
         *    ...->orderBy({"column1", "column2", ...})->...;
         *    ...->orderBy({"*"})->...;
         *   @endcode
         *   @retunrn Current SQLSelect pointer object to continue SQL query
         */
        SQLSelect* orderBy(const std::initializer_list<std::string>& columnList, bool desc = false)
        {
            privateCheckForLastSymbol();

            _queryStream << "order by ";
            for (auto& column : columnList)
            {
                _queryStream << column;
                _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
            }

            if (desc) _queryStream << " desc";

            return this;
        }
        /** @brief Method that select only distinct(different) values.
         *   @warning You must use it only before columns method!
         *   @return Current SQLSelect pointer object to continue SQL query
         */
        SQLSelect* distinct(void)
        {
            _queryStream << "distinct ";

            return this;
        }
        /** @brief Method that select join tables.
         *   @params join - join type (Check SQLJoinType enum)
         *    secondTableName - table with which will be join select
         *    onCondition - important condition for join tables
         *   @return Current SQLSelect pointer object to continue SQL query
         */
        SQLSelect* join(Utility::SQLJoinType join, const std::string& secondTableName,
                        const std::string& onCondition)
        {
            privateCheckForLastSymbol();

            switch (join)
            {
                case Utility::SQLJoinType::J_INNER:
                {
                    _queryStream << "inner join " << secondTableName;
                }
                break;
                case Utility::SQLJoinType::J_LEFT:
                {
                    _queryStream << "left join " << secondTableName;
                }
                break;
                case Utility::SQLJoinType::J_RIGHT:
                {
                    _queryStream << "right join " << secondTableName;
                }
                break;
                case Utility::SQLJoinType::J_FULL:
                {
                    _queryStream << "full join " << secondTableName;
                }
                break;
                default:
                {
                    throw Utility::OperationDBException("Unkown join type!", __FILE__, __LINE__);
                }
                break;
            }

            _queryStream << " on " << onCondition;

            return this;
        }
    
        /** @brief Method for group result table.
         *   @params columnList - list of the columns.
         *    Use ...({"*"})->... for all columns
         *   @return Current SQLSelect pointer object to continue SQL query
         */
        SQLSelect* groupBy(const std::initializer_list<std::string>& columnList)
        {
            privateCheckForLastSymbol();

            _queryStream << "group by ";

            for (auto& column : columnList)
            {
                _queryStream << column;
                _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
            }

            return this;
        }
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
        SQLSelect* having(const std::string& condition)
        {
            privateCheckForLastSymbol();

            _queryStream << "having " << condition;

            return this;
        }
        /** @brief Method for a condition between a single column
         *    and a range of other values (any of the values).
         *   @details It's equivolent of:
         *    x = ANY (a,b,c) -> x = a OR b OR c
         *   @params SQL subQuery string
         *   @return Current SQLSelect pointer object to continue SQL query
         */
        SQLSelect* Any(const std::string& subQuery)
        {
            privateCheckForLastSymbol();

            _queryStream << "any (" << condition << ")";

            return this;
        }
        /** @brief Method for a condition between a single column
         *    and a range of other values (a of the values).
         *   @params SQL subQuery string
         *   @return Current SQLSelect pointer object to continue SQL query
         */
        SQLSelect* All(const std::string& subQuery)
        {
            privateCheckForLastSymbol();

            _queryStream << "all (" << condition << ")";

            return this;
        }
    };

    /** @class SQLInsert.
     *   @brief SQLInsert class.
     *   @details It likes common SQL insert condition.
     */
    template <typename ResultType>
    class SQLInsert : public SQLBase<ResultType>
    {
    public:
        SQLInsert(QueryBuilder<ResultType>& table)
            : SQLBase<ResultType>(Utility::SQLStatement::ST_INSERT, table)
        {
        }

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
        template <typename... DataType>
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
        template <typename... DataType>
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
                [this](const auto&... tupleArgs) {
                    ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(tupleArgs) << ", "),
                     ...);
                },
                dataList);

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
        template <typename ColumnType = const char*, typename... DataType>
        SQLInsert* columns(const std::pair<ColumnType, DataType>&... columnData)
        {
            _queryStream << "(";
            ((_queryStream << columnData.first << ", "), ...);

            this->privateCorrectFormating();

            _queryStream << ")";

            _queryStream << " values(";
            ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(columnData.second) << ", "),
             ...);

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
        template <typename ColumnType = const char*, typename... DataType>
        SQLInsert* columns(const std::tuple<std::pair<ColumnType, DataType>...>& columnDataList)
        {
            _queryStream << "(";
            std::apply(
                [this](const auto&... tupleArgs) {
                    ((_queryStream << tupleArgs.first << ", "), ...);
                },
                columnDataList);

            this->privateCorrectFormating();

            _queryStream << ")";

            _queryStream << " values(";
            std::apply(
                [this](const auto&... tupleArgs) {
                    ((_queryStream << Utility::CheckForSQLSingleQuotesProblem(tupleArgs.second)
                                   << ", "),
                     ...);
                },
                columnDataList);

            this->privateCorrectFormating();

            _queryStream << ")";

            return this;
        }

    public:
        /** @brief Returning columns from table after insert.
         *   @params columnList - list of columns that need to return.
         *   @return Current SQLInsert pointer object to continue SQL query.
         */
        SQLInsert* returning(const std::initializer_list<std::string>& columnList)
        {
            privateCheckForLastSymbol();

            _queryStream << "returning ";
            for (auto& column : columnList)
            {
                _queryStream << column;
                _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
            }

            return this;
        }
    };

    /** @class SQLUpdate.
     *   @brief SQLUpdate class.
     *   @details It likes common SQL update condition.
     */
    template <typename ResultType>
    class SQLUpdate : public SQLBase<ResultType>, public SQLWhereCondition<SQLUpdate<ResultType>>
    {
    public:
        SQLUpdate(QueryBuilder<ResultType>& table)
            : SQLBase<ResultType>(Utility::SQLStatement::ST_UPDATE, table),
              SQLWhereCondition<SQLUpdate<ResultType>>(this)
        {
        }

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
        template <typename ColumnType = const char*, typename... Args>
        SQLUpdate* fields(const std::pair<ColumnType, Args>&... columnData)
        {
            _queryStream << " set ";

            ((_queryStream << columnData.first << " = "
                           << Utility::CheckForSQLSingleQuotesProblem(columnData.second) << ", "),
             ...);

            this->privateCorrectFormating();

            return this;
        }
        /** @brief Updating fields with new data.
         *   @params columnData - tuple of pairs.
         *   @return Current SQLUpdate pointer object to continue SQL query.
         */
        template <typename ColumnType = const char*, typename... Args>
        SQLUpdate* fields(const std::tuple<std::pair<ColumnType, Args>...>& columnData)
        {
            _queryStream << " set ";

            std::apply(
                [this](const auto&... tupleArg) {
                    ((_queryStream << tupleArg.first << " = "
                                   << Utility::CheckForSQLSingleQuotesProblem(tupleArg.second)
                                   << ", "),
                     ...);
                },
                columnData);

            this->privateCorrectFormating();

            return this;
        }
    };

    /** @class SQLDelete.
     *   @brief SQLDelete class.
     *   @details It likes common SQL delete condition.
     */
    template <typename ResultType>
    class SQLDelete : public SQLBase<ResultType>, public SQLWhereCondition<SQLDelete<ResultType>>
    {
    public:
        SQLDelete(QueryBuilder<ResultType>& table)
            : SQLBase<ResultType>(Utility::SQLStatement::ST_DELETE, table),
              SQLWhereCondition<SQLDelete<ResultType>>(this)
        {
        }

        virtual ~SQLDelete(void) = default;

    public:
        SQLDelete(void) = delete;

        SQLDelete(const SQLDelete&) = delete;
        SQLDelete& operator=(const SQLDelete&) = delete;

        SQLDelete(SQLDelete&&) = delete;
        SQLDelete& operator=(SQLDelete&&) = delete;
    };
}  // namespace DataAccess
