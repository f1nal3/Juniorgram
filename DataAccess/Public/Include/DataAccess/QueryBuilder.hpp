#pragma once

#include <DataAccess/IQuery.hpp>
#include <DataAccess/IBuilder.hpp>
#include <DataAccess/IAdapter.hpp>

#include <Utility/Exception.hpp>
#include <Utility/SQLUtility.hpp>
#include <Utility/Utility.hpp>

#include "SQLStatements.hpp"

namespace DataAccess
{
/**
* @class QueryBuilder
* @brief QueryBuilder class.
*   Class implements SQL queries, \
*   such as Select, Insert, Update, \
*   Delete and returns the corresponding object.
*/
template <typename ResultType>
class QueryBuilder : public IQuery<ResultType>, public IBuilder
{
private:
    Utility::DatabaseType _databaseType;
    std::string           _tableName;
    SQLBase<ResultType>*  _statement;

protected:
    std::shared_ptr<IAdapter> _adapter;

public:
    QueryBuilder(Utility::DatabaseType type, const std::string& tableName, std::shared_ptr<IAdapter> adapter)
        : _databaseType{type}, _tableName{tableName}, _statement{nullptr}, _adapter{adapter}
    {
    }

    virtual ~QueryBuilder() { this->clearStatement(); }

public:
    QueryBuilder() = delete;

    QueryBuilder(const QueryBuilder&)   = delete;
    QueryBuilder(QueryBuilder&&)        = delete;

    QueryBuilder& operator=(const QueryBuilder&)   = delete;
    QueryBuilder& operator=(QueryBuilder&&)        = delete;

public:
    /**
    * @brief SQL select query.
    * @return SQLSelect pointer.
    */
    SQLSelect<ResultType>* Select() override
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_SELECT)
            {
                throw Utility::OperationDBException
                (
                    "Previous query wasn't executed or rollbacked!",
                    __FILE__, __LINE__
                );
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLSelect<ResultType>>(*this);
            *_statement << " select ";
        }

        return dynamic_cast<SQLSelect<ResultType>*>(_statement);
    }

    /**
    * @brief SQL insert query.
    * @return SQLInsert pointer.
    */
    SQLInsert<ResultType>* Insert() override
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_INSERT)
            {
                throw Utility::OperationDBException
                (
                    "Previous query wasn't executed or rollbacked!", 
                    __FILE__, __LINE__
                );
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLInsert<ResultType>>(*this);
            *_statement << " insert into " << this->getCurrentTableName();
        }

        return dynamic_cast<SQLInsert<ResultType>*>(_statement);
    }

    /**
    * @brief SQL update query.
    * @return SQLUpdate pointer.
    */
    SQLUpdate<ResultType>* Update() override
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_UPDATE)
            {
                throw Utility::OperationDBException
                (
                    "Previous query wasn't executed or rollbacked!",
                    __FILE__, __LINE__
                );
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLUpdate<ResultType>>(*this);
            *_statement << " update " << this->getCurrentTableName();
        }

        return dynamic_cast<SQLUpdate<ResultType>*>(_statement);
    }

    /**
    * @brief SQL delete query.
    * @return SQLDelete pointer.
    */
    SQLDelete<ResultType>* Delete() override
    {
        if (_statement != nullptr)
        {
            if (_statement->getStatementType() != Utility::SQLStatement::ST_DELETE)
            {
                throw Utility::OperationDBException
                (
                    "Previous query wasn't executed or rollbacked!",
                    __FILE__, __LINE__
                );
            }
        }
        else
        {
            _statement = Utility::make_statement<SQLDelete<ResultType>>(*this);
            *_statement << " delete from " << this->getCurrentTableName();
        }

        return dynamic_cast<SQLDelete<ResultType>*>(_statement);
    }

public:
    /**
    * @brief Changing table.
    * @param newTableName - new name of the table.
    */
    void changeTable(const char* newTableName) noexcept override
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
    void changeTable(const std::string& newTableName) noexcept override
    {
        changeTable(newTableName.c_str()); 
    }

public:
    /**
    * @brief Get postgreAdapter object.
    * @return PostgreAdapter.
    */
    [[nodiscard]] std::shared_ptr<IAdapter> getAdapter() const noexcept override 
    {
        return _adapter; 
    }

    /**
    * @brief Get database type.
    * @return DatabaseType.
    */
    [[nodiscard]] Utility::DatabaseType getDatabaseType() const noexcept override
    {
        return _databaseType; 
    }

     /**
     * @brief Get current table name.
     * @return tableName.
     */
    [[nodiscard]] const std::string& getCurrentTableName() const noexcept override 
    {
        return _tableName; 
    }

    /**
    * @brief clear SQL statements
    */
    void clearStatement() override
    {
        delete _statement;
        _statement = nullptr;
    }
};
}  // namespace DataAccess
