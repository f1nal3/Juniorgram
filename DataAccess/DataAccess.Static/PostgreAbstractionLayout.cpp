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
            _insert->_queryStream << "insert into ";
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
   
    SQLSelect*                  SQLSelect::fields(const std::initializer_list<std::string>& columnList)
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
        _queryStream << " where " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::limit(std::uint32_t limit, std::uint32_t offset)
    {
        _queryStream << " limit " << limit << " offset " << offset;

        return this;
    }
    SQLSelect*                  SQLSelect::orderBy(const std::initializer_list<std::string>& columnList, bool desc)
    {
        _queryStream << " order by ";
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        if (desc) 
            _queryStream << " desc";

        return this;
    }
                                                
    SQLSelect*                  SQLSelect::And(const std::string& condition)
    {
        _queryStream << " and " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Or(const std::string& condition)
    {
        _queryStream << " or " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Not(const std::string& condition)
    {
        _queryStream << "not " << (condition == "" ? "" : condition);

        return this;
    }


    void                        SQLInsert::rollback(void) 
    { 
        _currentTable._clearI();
    }
    
    template<typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const DataType&... data)
    {
        Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);

        return this;
    }
    template<typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const std::tuple<DataType...>& dataList)
    {
        Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
    
        return this;
    }
    template<typename ColumnType ,typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const std::pair<ColumnType, DataType>&... columnData)
    {
        Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);

        return this;
    }
    template<typename ColumnType ,typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const std::tuple<std::pair<ColumnType, DataType>...>& columnDataList)
    {
        Utility::NotImplementedException("Not implemented.", __FILE__, __LINE__);
    
        return this;
    }

    SQLInsert*                  SQLInsert::returning(const std::initializer_list<std::string>& columnList)
    {
        _queryStream << " returning ";
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
}
