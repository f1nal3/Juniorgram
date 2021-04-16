#include "PostgreAbstractionLayout.hpp"

#include <iostream>

namespace DataAccess
{
    SQLSelect*  Table::Select(void)
    {
        if (_select == nullptr)
        {
            _select = new SQLSelect(*this);
            _select->_queryStream << "select ";
        }

        return _select;
    }
    SQLInsert*  Table::Insert(void)
    {
        if (_insert == nullptr)
        {
            _insert = new SQLInsert(*this);
            _insert->_queryStream << "insert into " << _tableName;
        }

        return _insert;
    }
    SQLUpdate*  Table::Update(void)
    { 
        if (_update == nullptr)
        {
            _update = new SQLUpdate(*this);
            _update->_queryStream << "insert into " << _tableName;
        }

        return _update;
    }
    SQLDelete*  Table::Delete(void)
    {
        if (_delete == nullptr)
        {
            _delete = new SQLDelete(*this);
            _delete->_queryStream << "insert into " << _tableName;
        }

        return _delete;
    }

    void                        Table::changeTable(const char* newTableName) noexcept 
    {
        _select->rollback();
        _insert->rollback();
        _update->rollback();
        _delete->rollback();

        _tableName = newTableName;
    }
    void                        Table::changeTable(const std::string& newTableName) noexcept 
    { 
        changeTable(newTableName.c_str());
    }
    
    void                        Table::_clear(SQLStatement statement)
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

    Table::~Table(void)
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


    SQLStatement                SQLBase::type(void) const noexcept
    {
        return _statement; 
    }
    
    const std::string           SQLBase::getQuery(void) const noexcept
    {
        return _queryStream.str();
    }
    std::optional<pqxx::result> SQLBase::execute(void) 
    {
        auto result = _currentTable._postgre->query(_queryStream.str() + ";");

        this->rollback();

        if (result.has_value())
            return result;

        return std::nullopt;
    }

    void                        SQLBase::rollback(void)
    { 
        _currentTable._clear(_statement);
    }


    SQLSelect*                  SQLSelect::columns(const std::initializer_list<std::string>& columnList)
    {
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }
        _queryStream << " from " << _currentTable._tableName;

        return this;
    }                            

    SQLSelect*                  SQLSelect::limit(std::uint32_t limit, std::uint32_t offset)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "limit " << limit << " offset " << offset;

        return this;
    }
    SQLSelect*                  SQLSelect::orderBy(const std::initializer_list<std::string>& columnList, bool desc)
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
    SQLSelect*                  SQLSelect::distinct(void)
    { 
        _queryStream << "distinct ";

        return this;
    }

    SQLSelect*                  SQLSelect::groupBy(const std::initializer_list<std::string>& columnList)
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
    SQLSelect*                  SQLSelect::having(const std::string& condition)
    { 
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "having " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Any(const std::string& condition)
    { 
        if (*(_queryStream.str().end() - 1) != ' ') 
            _queryStream << " ";

        _queryStream << "any (" << condition << ")";

        return this;
    }
    SQLSelect*                  SQLSelect::All(const std::string& condition) 
    { 
        if (*(_queryStream.str().end() - 1) != ' ') 
            _queryStream << " ";

        _queryStream << "all (" << condition << ")";

        return this;
    }


    SQLInsert*                  SQLInsert::returning(const std::initializer_list<std::string>& columnList)
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

    void                        SQLInsert::_correctFormating()
    {
        std::string queryBuffer = _queryStream.str();
        queryBuffer.erase(queryBuffer.end() - 2, queryBuffer.end());

        _queryStream.str(queryBuffer);
        _queryStream.seekp(0, std::ios_base::end);
    }
}
