#include "DatabaseAbstractionLayout.hpp"

#include <iostream>

namespace DataAccess
{
    SQLSelect*                            QueryCreator::Select(void)
    {
        if (_select == nullptr)
        {
            _select = new SQLSelect(*this);
            _select->_queryStream << "select ";
        }

        return _select;
    }
    SQLInsert*                            QueryCreator::Insert(void)
    {
        if (_insert == nullptr)
        {
            _insert = new SQLInsert(*this);
            _insert->_queryStream << "insert into " << _tableName;
        }

        return _insert;
    }
    SQLUpdate*                            QueryCreator::Update(void)
    { 
        if (_update == nullptr)
        {
            _update = new SQLUpdate(*this);
            _update->_queryStream << "update " << _tableName;
        }

        return _update;
    }
    SQLDelete*                            QueryCreator::Delete(void)
    {
        if (_delete == nullptr)
        {
            _delete = new SQLDelete(*this);
            _delete->_queryStream << "delete from " << _tableName;
        }

        return _delete;
    }

    void                                  QueryCreator::changeTable(const char* newTableName) noexcept 
    {
        _select->rollback();
        _insert->rollback();
        _update->rollback();
        _delete->rollback();

        _tableName = newTableName;
    }
    void                                  QueryCreator::changeTable(const std::string& newTableName) noexcept 
    { 
        changeTable(newTableName.c_str());
    }
    
    std::shared_ptr<IAdapter>             QueryCreator::getAdapter(void) const noexcept
    {
        return _adapter;
    }

    void                                  QueryCreator::privateClear(SQLStatement statement)
    {
        switch (statement)
        {
            case SQLStatement::ST_SELECT:
            {
                delete _select;
                _select = nullptr;
            }
            break;
            case SQLStatement::ST_INSERT:
            {
                delete _insert;
                _insert = nullptr;
            }
            break;
            case SQLStatement::ST_UPDATE:
            {
                delete _update;
                _update = nullptr;
            }
            break;
            case SQLStatement::ST_DELETE:
            {
                delete _delete;
                _delete = nullptr;
            }
            break;
            default:
                break;
        }
    }

    QueryCreator::~QueryCreator(void)
    {
        delete _select;
        delete _insert;
        delete _update;
        delete _delete;

        _select = nullptr;
        _insert = nullptr;
        _update = nullptr;
        _delete = nullptr;
    }


    SQLStatement                          SQLBase::type(void) const noexcept
    {
        return _statement; 
    }
    const std::string                     SQLBase::getQuery(void) const noexcept
    {
        return _queryStream.str();
    }
    void                                  SQLBase::rollback(void)
    { 
        _currentCreator.privateClear(_statement);
    }


    SQLSelect*                            SQLSelect::columns(const std::initializer_list<std::string>& columnList)
    {
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }
        _queryStream << " from " << _currentCreator._tableName;

        return this;
    }                            

    SQLSelect*                            SQLSelect::limit(std::uint32_t limit, std::uint32_t offset)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "limit " << limit << " offset " << offset;

        return this;
    }
    SQLSelect*                            SQLSelect::orderBy(const std::initializer_list<std::string>& columnList, bool desc)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "order by ";
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        if (desc) 
            _queryStream << " desc";

        return this;
    }
    SQLSelect*                            SQLSelect::distinct(void)
    { 
        _queryStream << "distinct ";

        return this;
    }
    SQLSelect*                            SQLSelect::join(SQLJoinType join, const std::string& secondTableName, const std::string& onCondition)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        switch (join)
        {
            case DataAccess::SQLJoinType::J_INNER:
            {
                _queryStream << "join " << secondTableName;
            }
            break;
            case DataAccess::SQLJoinType::J_LEFT:
            {
                _queryStream << "left join " << secondTableName;
            }
            break;
            case DataAccess::SQLJoinType::J_RIGHT:
            {
                _queryStream << "right join " << secondTableName;
            }
            break;
            case DataAccess::SQLJoinType::J_FULL:
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
    SQLSelect*                            SQLSelect::groupBy(const std::initializer_list<std::string>& columnList)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "group by ";

        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
    SQLSelect*                            SQLSelect::having(const std::string& condition)
    { 
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "having " << condition;

        return this;
    }
    SQLSelect*                            SQLSelect::Any(const std::string& condition)
    { 
        if (*(_queryStream.str().end() - 1) != ' ') 
            _queryStream << " ";

        _queryStream << "any (" << condition << ")";

        return this;
    }
    SQLSelect*                            SQLSelect::All(const std::string& condition) 
    { 
        if (*(_queryStream.str().end() - 1) != ' ') 
            _queryStream << " ";

        _queryStream << "all (" << condition << ")";

        return this;
    }


    SQLInsert*                            SQLInsert::returning(const std::initializer_list<std::string>& columnList)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "returning ";
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }

    void                                  SQLInsert::privateCorrectFormating(void)
    {
        std::string queryBuffer = _queryStream.str();
        queryBuffer.erase(queryBuffer.end() - 2, queryBuffer.end());

        _queryStream.str(queryBuffer);
        _queryStream.seekp(0, std::ios_base::end);
    }


    void                                  SQLUpdate::privateCorrectFormating(void)
    {
        std::string queryBuffer = _queryStream.str();
        queryBuffer.erase(queryBuffer.end() - 2, queryBuffer.end());

        _queryStream.str(queryBuffer);
        _queryStream.seekp(0, std::ios_base::end);
    }
}
