#pragma once

#include <iostream>
#include <string>

#include "IAdapter.hpp"
#include "SQLStatements.hpp"

namespace DataAccess
{
/**
 * @class QueryBuilder
 * @brief Supportive class for constructing SQL query.
 * @details The class forms and saves SQL query, current
 * table and type of database. Class provides interface, which is as close as possible
 * to the classical SQL queries. This is achieved by calling methods one by one.
 * Examples for postgre database you can see in PostgreQuery.hpp
 */
template <typename ResultType>
class AbstractQueryBuilder
{
private:
    Utility::DatabaseType     _databaseType;
    std::string               _tableName;
    SQLBase<ResultType>*      _statement;
    std::shared_ptr<IAdapter> _adapter;

public:
    AbstractQueryBuilder(Utility::DatabaseType type, const std::string& tableName, std::shared_ptr<IAdapter> adapter)
        : _databaseType{type}, _tableName{tableName}, _statement{nullptr}, _adapter{adapter}
    {
    }

    virtual ~AbstractQueryBuilder(void) { this->clearStatement(); }

public:
    AbstractQueryBuilder() = delete;

    AbstractQueryBuilder(const AbstractQueryBuilder&) = delete;
    AbstractQueryBuilder(AbstractQueryBuilder&&)      = delete;

    AbstractQueryBuilder& operator=(const AbstractQueryBuilder&) = delete;
    AbstractQueryBuilder& operator=(AbstractQueryBuilder&&)      = delete;

public:
    /**
     * @brief SQL select query.
     * @return SQLSelect pointer.
     */
    SQLSelect<ResultType>* Select(void)
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_SELECT)
            {
                throw Utility::OperationDBException("Previous query wasn't executed or rollbacks!", __FILE__, __LINE__);
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLSelect<ResultType>>(*this);
            *_statement << "select ";
        }

        return dynamic_cast<SQLSelect<ResultType>*>(_statement);
    }

    /**
     * @brief SQL insert query.
     * @return SQLInsert pointer.
     */
    SQLInsert<ResultType>* Insert(void)
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_INSERT)
            {
                throw Utility::OperationDBException("Previous query wasn't executed or rollbacks!", __FILE__, __LINE__);
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLInsert<ResultType>>(*this);
            *_statement << "insert into " << this->getCurrentTableName();
        }

        return dynamic_cast<SQLInsert<ResultType>*>(_statement);
    }

    /**
     * @brief SQL update query.
     * @return SQLUpdate pointer.
     */
    SQLUpdate<ResultType>* Update(void)
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_UPDATE)
            {
                throw Utility::OperationDBException("Previous query wasn't executed or rollbacks!", __FILE__, __LINE__);
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLUpdate<ResultType>>(*this);
            *_statement << "update " << this->getCurrentTableName();
        }

        return dynamic_cast<SQLUpdate<ResultType>*>(_statement);
    }

    /**
     * @brief SQL delete query.
     * @return SQLDelete pointer.
     */
    SQLDelete<ResultType>* Delete(void)
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_DELETE)
            {
                throw Utility::OperationDBException("Previous query wasn't executed or rollbacks!", __FILE__, __LINE__);
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLDelete<ResultType>>(*this);
            *_statement << "delete from " << this->getCurrentTableName();
        }

        return dynamic_cast<SQLDelete<ResultType>*>(_statement);
    }

public:
    /**
     * @brief Changing table.
     * @param newTableName - new name of the table.
     */
    void changeTable(const char* newTableName) noexcept
    {
        if (_statement != nullptr)
        {
            _statement->rollback();
        }

        _tableName = newTableName;
    }

    /**
     * @brief Changing table.
     * @param newTableName - new name of the table.
     */
    void changeTable(const std::string& newTableName) noexcept { changeTable(newTableName.c_str()); }

public:
    /**
     * @brief Get postgreAdapter object.
     * @return PostgreAdapter.
     */
    [[nodiscard]] std::shared_ptr<IAdapter> getAdapter(void) const noexcept { return _adapter; }

    [[nodiscard]] Utility::DatabaseType getDatabaseType(void) const noexcept { return _databaseType; }

    [[nodiscard]] const std::string& getCurrentTableName(void) const noexcept { return _tableName; }

    void clearStatement()
    {
        delete _statement;
        _statement = nullptr;
    }
};
}  // namespace DataAccess
