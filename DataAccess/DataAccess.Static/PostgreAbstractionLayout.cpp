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
        return _update;
    }
    SQLDelete*  Table::Delete(void)
    {
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
    
    Table::~Table()
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

    
    void                        SQLSelect::rollback(void) 
    { 
        _currentTable._clearS();
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

    SQLSelect*                  SQLSelect::where(const std::string& condition)
    { 
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "where " << condition;

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

    SQLSelect*                  SQLSelect::And(const std::string& condition)
    {
       if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "and " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Or(const std::string& condition)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "or " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Not(const std::string& condition)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "not " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::In(const std::string& anotherStatement)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";

        _queryStream << "in (" << anotherStatement << ")";

        return this;
    }
    SQLSelect*                  SQLSelect::In(const std::initializer_list<std::string>& valueList)
    {
        if (*(_queryStream.str().end() - 1) != ' ')
            _queryStream << " ";
        _queryStream << "in (";

        for (auto& value : valueList)
        {
            _queryStream << value;
            _queryStream << (value != *(valueList.end() - 1) ? ", " : "");
        }

        _queryStream << ")";

        return this;
    }


    void                        SQLInsert::rollback(void) 
    { 
        _currentTable._clearI();
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
