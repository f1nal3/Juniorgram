#pragma once
#include <any>
#include <iostream>
#include <optional>
#include <sstream>

#include <Utility/Exception.hpp>
#include <Utility/SQLUtility.hpp>

#include "FileLogger.hpp"

namespace DataAccess
{
template <typename ResultType>
class QueryBuilder;

/**
 * @class SQLWhereCondition
 * @brief SQLWhereCondition class.
 * @details This class is a secondary. It needs for \
 *    SQLSelect, SQLUpdate and SQLUpdate. \
 *    This class using for 'where' condition from SQL
 * @warning You shouldn't use it itself. Only from QueryCreator class.
 */
template <class T>
class SQLWhereCondition
{
private:
    T* _statement;

public:
    explicit SQLWhereCondition(T* statement) : _statement{statement} {}

    virtual ~SQLWhereCondition(void) = default;

public:
    /**
     * @brief Like common SQL 'where' condition.
     * @param condition - SQL condition, but it is not required \
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
     */
    T* Where(const std::string& condition = {})
    {
        privateStatementCorrectness();

        *_statement << "where " << condition;

        return _statement;
    }
    /**
     * @brief Like common SQL 'and' condition. This condition \
     *    need for continue the SQL query after like: limit, beetween, etc.
     * @param condition - SQL condition, but it is not required. \
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
     */
    T* And(const std::string& condition = {})
    {
        privateStatementCorrectness();

        *_statement << "and " << condition;

        return _statement;
    }
    /**
     * @brief Like common SQL 'or' condition. This condition \
     *   need for continue the SQL query after like: limit, beetween, etc.
     * @param condition - SQL condition, but it is not required. \
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
     */
    T* Or(const std::string& condition = {})
    {
        privateStatementCorrectness();

        *_statement << "or " << condition;

        return _statement;
    }
    /**
     * @brief In condition.
     * @param anotherStatement - another SQL statement, \
     *  that you can receive from 'getQuery()' method.
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
     */
    T* In(const std::string& anotherStatement)
    {
        privateStatementCorrectness();

        *_statement << "in (" << anotherStatement << ")";

        return _statement;
    }
    /**
     * @brief In condition.
     * @param valueList - checking from list of values.
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
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
    /**
     * @brief Beeween condition.
     * @param left - start of the range.
     *  right - end of the range.
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
     */
    template <typename ValueType>
    T* Between(ValueType left, ValueType right)
    {
        privateStatementCorrectness();

        *_statement << "between " << left << " and " << right;

        return _statement;
    }
    /**
     * @brief Like condition.
     * @param pattern - see this for info: \
     *    https://www.w3schools.com/sql/sql_like.asp, \
     *    https://www.w3schools.com/sql/sql_wildcards.asp
     * @return Current SQLSTATEMENT pointer object to continue SQL query.
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

/**
 * @class SQLBase
 * @brief SQLBase abstraction class.
 * @details This class has 4 non-overloaded inherited methods.
 * @warning Like a SQLWhereCondition
 * you shouldn't use it itself.
 */
template <typename ResultType>
class SQLBase
{
protected:
    Utility::SQLStatement     _statementType;
    QueryBuilder<ResultType>& _currentBuilder;
    std::ostringstream        _queryStream;

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
    SQLBase& operator<<(T&& data)
    {
        _queryStream << std::forward<T>(data);

        return *this;
    }

public:
    /**
     * @brief Return SQL statement type.
     * @return ST_SELECT, ST_INSERT, ST_UPDATE, ST_DELETE
     */
    Utility::SQLStatement getStatementType(void) const noexcept  { return _statementType; }
    /**
     * @brief Method that returns current SQL query string.
     *  This method for subqueries basically.
     * @return SQL query string
     * @warning Don't clear SQL string if you use it.
     *    After getQuery you whatever can call method execute.
     * @code
     *    QueryBuilder table("tableName");
     *    table.'SQLSTATEMENT'()->getQuery();
     *    table.'SQLSTATEMENT'()->execute(); /// Here SQL query string will be clear.
     * @encode
     */
    std::string getStringQuery(void) const noexcept { return _queryStream.str(); }
    /**
     * @brief Method that executes SQL string.
     * For technical reasons,
     *   you must specify the type of container
     *   where you want to put the result:
     *   pqxx::result or SQLiteResultType.
     *   Maybe it can be fixed in the near future.
     * @warning Be careful with timestamps. That can only be c_str!!!
     * @details This method clear SQL query string after call it.
     * @return Optional object pqxx::result or SQLiteResultObject
     *    For pqxx::result check here:
     *    https://libpqxx.readthedocs.io/en/6.4/a01127.html
     */
    std::optional<ResultType> execute(void)
    {
        try
        {
            if (_currentBuilder.getAdapter()->isConnected())
            {
                std::optional<std::any> result = _currentBuilder.getAdapter()->query(SQLBase<ResultType>::_queryStream.str());

                if (result.has_value())
                {
                    this->rollback();

                    return {std::any_cast<ResultType>(result.value())};
                }
            }
            else
            {
                throw Utility::OperationDBException("Database connection failure!", __FILE__, __LINE__);
            }
        }

        catch (const std::exception& err)
        {
            Base::Logger::FileLogger::getInstance().log(
                std::string(err.what() + '\n'),
                Base::Logger::LogLevel::ERR);
        }

        this->rollback();

        return std::nullopt;
    }
    /**
     * @brief Method that clear SQL query string.
     * @code
     *  QueryBuilder table("tableName");
     *  table.'SQLSTATEMENT'()->qetQuery();
     *  table.'SQLSTATEMENT'()->rollback();
     * @endcode
     */
    void rollback(void) { SQLBase<ResultType>::_currentBuilder.clearStatement(); }

protected:
    void privateCheckForLastSymbol(void)
    {
        if (*(SQLBase<ResultType>::_queryStream.str().end() - 1) != ' ') SQLBase<ResultType>::_queryStream << " ";
    }

    void privateCorrectFormating(void)
    {
        std::string queryBuffer = SQLBase<ResultType>::_queryStream.str();
        queryBuffer.erase(queryBuffer.end() - 2, queryBuffer.end());

        SQLBase<ResultType>::_queryStream.str(queryBuffer);
        SQLBase<ResultType>::_queryStream.seekp(0, std::ios_base::end);
    }
};

/**
 * @class SQLSelect
 * @brief SQLSelect class.
 * @details It likes common SQL select condition.
 * @warning After call it you must call the method 'columns'
 *    (BUT distinct is exception)
 *    and give it columns that you need to return.
 *    OR you can give '*'.
 */
template <typename ResultType>
class SQLSelect : public SQLBase<ResultType>, public SQLWhereCondition<SQLSelect<ResultType>>
{
public:
    SQLSelect(QueryBuilder<ResultType>& table)
        : SQLBase<ResultType>(Utility::SQLStatement::ST_SELECT, table), SQLWhereCondition<SQLSelect<ResultType>>(this)
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
    /**
     * @brief Columns that you need to return from database.
     * @details You must point columns that you need, like:
     * @code
     *    ...->columns({"column1", "column2", ...})->...;
     * @endcode
     * @details Or, for all columns from table use:
     * @code
     *   ...->columns({"*"})->...;
     * @param Inintializer_list<string> of columns that you need.
     * @return Current SQLSelect pointer object to continue SQL query.
     */
    SQLSelect* columns(const std::initializer_list<std::string>& columnList)
    {
        for (auto& column : columnList)
        {
            SQLBase<ResultType>::_queryStream << column;
            SQLBase<ResultType>::_queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }
        SQLBase<ResultType>::_queryStream << " from " << SQLBase<ResultType>::_currentBuilder.getCurrentTableName();

        return this;
    }

public:
    /**
     * @brief Method for limiting returned SQL rows.
     * @details For example limit 5 offset 2 return: \
     *    row3, row4, row5, row6, row7.
     * @param limit - how much rows that you need
     *    offset - from what row start limit
     * @return Current SQLSelect pointer object to continue SQL query.
     */
    SQLSelect* limit(std::uint32_t limit, std::uint32_t offset = {})
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "limit " << limit << " offset " << offset;

        return this;
    }
    /**
     * @brief Method for selecting rows by columns in alphabetic order.
     * @details Use desc = true, if you wanna select in reverse order.
     * @param columnList - list of columns.
     * @param desc - reverse order.
     * @code
     *    ...->orderBy({"column1", "column2", ...})->...;
     *    ...->orderBy({"*"})->...;
     * @endcode
     * @retunrn Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* orderBy(const std::initializer_list<std::string>& columnList, bool desc = false)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "order by ";
        for (auto& column : columnList)
        {
            SQLBase<ResultType>::_queryStream << column;
            SQLBase<ResultType>::_queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        if (desc) SQLBase<ResultType>::_queryStream << " desc";

        return this;
    }
    /**
     * @brief Method that select only distinct(different) values.
     * @warning You must use it only before columns method!
     * @return Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* distinct(void)
    {
        SQLBase<ResultType>::_queryStream << "distinct ";

        return this;
    }
    /**
     * @brief Method that select join tables.
     * @param join - join type (Check SQLJoinType enum)
     *  secondTableName - table with which will be join select
     *  onCondition - important condition for join tables
     * @return Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* join(Utility::SQLJoinType join, const std::string& secondTableName, const std::string& onCondition)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        switch (join)
        {
            case Utility::SQLJoinType::J_INNER:
            {
                SQLBase<ResultType>::_queryStream << "inner join " << secondTableName;
            }
            break;
            case Utility::SQLJoinType::J_LEFT:
            {
                SQLBase<ResultType>::_queryStream << "left join " << secondTableName;
            }
            break;
            case Utility::SQLJoinType::J_RIGHT:
            {
                SQLBase<ResultType>::_queryStream << "right join " << secondTableName;
            }
            break;
            case Utility::SQLJoinType::J_FULL:
            {
                SQLBase<ResultType>::_queryStream << "full join " << secondTableName;
            }
            break;
            default:
            {
                throw Utility::OperationDBException("Unkown join type!", __FILE__, __LINE__);
            }
            break;
        }

        SQLBase<ResultType>::_queryStream << " on " << onCondition;

        return this;
    }

    /**
     * @brief Method for group result table.
     * @param columnList - list of the columns.
     *    Use ...({"*"})->... for all columns
     * @return Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* groupBy(const std::initializer_list<std::string>& columnList)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "group by ";

        for (auto& column : columnList)
        {
            SQLBase<ResultType>::_queryStream << column;
            SQLBase<ResultType>::_queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
    /**
     * @brief Method for using instead of where with aggregate functions.
     * @code
     *    SELECT Employees.LastName, COUNT(Orders.OrderID) AS NumberOfOrders
     *    FROM Orders
     *    INNER JOIN Employees ON Orders.EmployeeID = Employees.EmployeeID
     *    WHERE LastName = 'Davolio' OR LastName = 'Fuller'
     *    GROUP BY LastName
     *    HAVING COUNT(Orders.OrderID) > 25;
     * @endcode
     * @param condition - string SQL condition
     * @return Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* having(const std::string& condition)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "having " << condition;

        return this;
    }
    /**
     * @brief Method for a condition between a single column
     *  and a range of other values (any of the values).
     * @details It's equivolent of:
     *    x = ANY (a,b,c) -> x = a OR b OR c
     * @param SQL subQuery string
     * @return Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* Any(const std::string& subQuery)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "any (" << subQuery << ")";

        return this;
    }
    /**
     * @brief Method for a condition between a single column
     *    and a range of other values (a of the values).
     * @param SQL subQuery string
     * @return Current SQLSelect pointer object to continue SQL query
     */
    SQLSelect* All(const std::string& subQuery)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "all (" << subQuery << ")";

        return this;
    }
};

/**
 * @class SQLInsert
 * @brief SQLInsert class.
 * @details It likes common SQL insert condition.
 */
template <typename ResultType>
class SQLInsert : public SQLBase<ResultType>
{
public:
    SQLInsert(QueryBuilder<ResultType>& table) : SQLBase<ResultType>(Utility::SQLStatement::ST_INSERT, table) {}

    virtual ~SQLInsert(void) = default;

public:
    SQLInsert(void) = delete;

    SQLInsert(const SQLInsert&) = delete;
    SQLInsert& operator=(const SQLInsert&) = delete;

    SQLInsert(SQLInsert&&) = delete;
    SQLInsert& operator=(SQLInsert&&) = delete;

public:
    /**
     * @brief Insert one row into table.
     * @details Inserting all columns into a row.
     * @code
     *    ...->field(1, "a", "male")->field(2, "b", "female")...;
     * @endcode
     * @param list of row's columns.
     * @return Current SQLInsert pointer object to continue SQL query.
     */
    template <typename... DataType>
    SQLInsert* field(const DataType&... data)
    {
        if (SQLBase<ResultType>::_queryStream.str().find("values") == std::string::npos)
        {
            SQLBase<ResultType>::_queryStream << " values(";
        }
        else
        {
            SQLBase<ResultType>::_queryStream << ", (";
        }

        ((SQLBase<ResultType>::_queryStream << Utility::CheckForSQLSingleQuotesProblem(data) << ", "), ...);

        this->SQLBase<ResultType>::privateCorrectFormating();

        SQLBase<ResultType>::_queryStream << ")";

        return this;
    }
    /**
     * @brief Insert one row into table.
     * @details Inserting columns into a row as tuple. \
     * @code
     *   ...->field(tupleName1)->field(tupleName2)...;
     * @endcode
     * @param Taple with data.
     * @return Current SQLInsert pointer object to continue SQL query.
     */
    template <typename... DataType>
    SQLInsert* field(const std::tuple<DataType...>& dataList)
    {
        if (SQLBase<ResultType>::_queryStream.str().find("values") == std::string::npos)
        {
            SQLBase<ResultType>::_queryStream << " values(";
        }
        else
        {
            SQLBase<ResultType>::_queryStream << ", (";
        }

        std::apply(
            [this](const auto&... tupleArgs) {
                ((SQLBase<ResultType>::_queryStream << Utility::CheckForSQLSingleQuotesProblem(tupleArgs) << ", "), ...);
            },
            dataList);

        this->SQLBase<ResultType>::privateCorrectFormating();

        SQLBase<ResultType>::_queryStream << ")";

        return this;
    }
    /**
     * @brief Insert rows into table.
     * @details Inserting several columns into a row as pairs. \
     *   You don't have to wrap columnName by quotes, \
     *   BUT you must wrap data strings.
     * @code
     *   ...->columns(pair{"Column1", 1}, pair{"Column2", "data"})->field(data)->...;
     * @endcode
     * @param Pairs.
     * @return Current SQLInsert pointer object to continue SQL query.
     */
    template <typename ColumnType = const char*, typename... DataType>
    SQLInsert* columns(const std::pair<ColumnType, DataType>&... columnData)
    {
        SQLBase<ResultType>::_queryStream << "(";
        ((SQLBase<ResultType>::_queryStream << columnData.first << ", "), ...);

        this->SQLBase<ResultType>::privateCorrectFormating();

        SQLBase<ResultType>::_queryStream << ")";

        SQLBase<ResultType>::_queryStream << " values(";
        ((SQLBase<ResultType>::_queryStream << Utility::CheckForSQLSingleQuotesProblem(columnData.second) << ", "), ...);

        this->SQLBase<ResultType>::privateCorrectFormating();

        SQLBase<ResultType>::_queryStream << ")";

        return this;
    }
    /**
     * @brief Insert rows into table.
     * @details Inserting several columns into a row as tuple of pairs. \
     *   You don't have to wrap columnName by quotes. \
     * @code
     *   ...->columns(tupleName1)->field(data)->...;
     * @endcode
     * @param Tuple of pairs.
     * @return Current SQLInsert pointer object to continue SQL query.
     */
    template <typename ColumnType = const char*, typename... DataType>
    SQLInsert* columns(const std::tuple<std::pair<ColumnType, DataType>...>& columnDataList)
    {
        SQLBase<ResultType>::_queryStream << "(";
        std::apply([this](const auto&... tupleArgs) { ((SQLBase<ResultType>::_queryStream << tupleArgs.first << ", "), ...); },
                   columnDataList);

        this->SQLBase<ResultType>::privateCorrectFormating();

        SQLBase<ResultType>::_queryStream << ")";

        SQLBase<ResultType>::_queryStream << " values(";
        std::apply(
            [this](const auto&... tupleArgs) {
                ((SQLBase<ResultType>::_queryStream << Utility::CheckForSQLSingleQuotesProblem(tupleArgs.second) << ", "), ...);
            },
            columnDataList);

        this->SQLBase<ResultType>::privateCorrectFormating();

        SQLBase<ResultType>::_queryStream << ")";

        return this;
    }

public:
    /**
     * @brief Returning columns from table after insert.
     * @param columnList - list of columns that need to return.
     * @return Current SQLInsert pointer object to continue SQL query.
     */
    SQLInsert* returning(const std::initializer_list<std::string>& columnList)
    {
        SQLBase<ResultType>::privateCheckForLastSymbol();

        SQLBase<ResultType>::_queryStream << "returning ";
        for (auto& column : columnList)
        {
            SQLBase<ResultType>::_queryStream << column;
            SQLBase<ResultType>::_queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
};

/**
 * @class SQLUpdate
 * @brief SQLUpdate class.
 * @details It likes common SQL update condition.
 */
template <typename ResultType>
class SQLUpdate : public SQLBase<ResultType>, public SQLWhereCondition<SQLUpdate<ResultType>>
{
public:
    SQLUpdate(QueryBuilder<ResultType>& table)
        : SQLBase<ResultType>(Utility::SQLStatement::ST_UPDATE, table), SQLWhereCondition<SQLUpdate<ResultType>>(this)
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
    /**
     * @brief Updating fields with new data.
     * @param columnData - pairs with column names by first \
     *  and the data by second.
     * @return Current SQLUpdate pointer object to continue SQL query.
     */
    template <typename ColumnType = const char*, typename... Args>
    SQLUpdate* fields(const std::pair<ColumnType, Args>&... columnData)
    {
        SQLBase<ResultType>::_queryStream << " set ";

        ((SQLBase<ResultType>::_queryStream << columnData.first << " = " << Utility::CheckForSQLSingleQuotesProblem(columnData.second)
                                            << ", "),
         ...);

        this->SQLBase<ResultType>::privateCorrectFormating();

        return this;
    }
    /**
     * @brief Updating fields with new data.
     * @param columnData - tuple of pairs.
     * @return Current SQLUpdate pointer object to continue SQL query.
     */
    template <typename ColumnType = const char*, typename... Args>
    SQLUpdate* fields(const std::tuple<std::pair<ColumnType, Args>...>& columnData)
    {
        SQLBase<ResultType>::_queryStream << " set ";

        std::apply(
            [this](const auto&... tupleArg) {
                ((SQLBase<ResultType>::_queryStream << tupleArg.first << " = " << Utility::CheckForSQLSingleQuotesProblem(tupleArg.second)
                                                    << ", "),
                 ...);
            },
            columnData);

        this->SQLBase<ResultType>::privateCorrectFormating();

        return this;
    }
};

/**
 * @class SQLDelete
 * @brief SQLDelete class.
 * @details It likes common SQL delete condition.
 */
template <typename ResultType>
class SQLDelete : public SQLBase<ResultType>, public SQLWhereCondition<SQLDelete<ResultType>>
{
public:
    explicit SQLDelete(QueryBuilder<ResultType>& table)
        : SQLBase<ResultType>(Utility::SQLStatement::ST_DELETE, table), SQLWhereCondition<SQLDelete<ResultType>>(this)
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
}  /// namespace DataAccess
