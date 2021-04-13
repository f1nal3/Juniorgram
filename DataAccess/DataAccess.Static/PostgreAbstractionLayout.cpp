#include "PostgreAbstractionLayout.hpp"

namespace DataAccess
{
    SQLSelect*                  Table::Select(void)
    {
        if (_select != nullptr)
        {
            throw Utility::OperationDBException(
                "The previous select statement wasn't executed!", __FILE__, __LINE__);
        }

        _select = new SQLSelect(*this);
        _select->_queryStream << "select ";

        return _select;
    }
    SQLInsert*                  Table::Insert(void)
    {
        if (_insert != nullptr)
        {
            throw Utility::OperationDBException(
                "The previous select statement wasn't executed!", __FILE__, __LINE__);
        }

        _insert = new SQLInsert(*this);
        _insert->_queryStream << "insert into ";

        return _insert;
    }
    //SQLInsert* Table::Insert(void)
    //{ 
    //    if (_insert != nullptr)
    //    {
    //        throw Utility::EncorrectOperationDBException(
    //            "The previous select statement wasn't executed!", __FILE__, __LINE__);
    //    }
    //
    //
    //
    //    return _insert;
    //}
    //SQLUpdate* Table::Update(void) 
    //{ 
    //    return _update;
    //}
    //SQLDelete* Table::Delete(void)
    //{ 
    //    return _delete;
    //}


    SQLStatement                SQLBase::type(void) const
    {
        return _statement; 
    }
    
    void                        SQLBase::rollback(void)
    {
        _queryStream.str("");
        _queryStream.clear();
    }
    const std::string&          SQLBase::getQuery(void) const
    {
        return _queryStream.str();
    }
    std::optional<pqxx::result> SQLBase::execute(void)
    {
        auto result = _currentTable._postgre->query(_queryStream.str() + ";");

        delete this;
        this->rollback();

        if (result.has_value())
            return result;

        return std::nullopt;
    }


    SQLSelect*                  SQLSelect::fields(const std::initializer_list<std::string>& columnList)
    {
        if (*(_queryStream.str().end() - 1) != ',')
        {
            _queryStream << ", ";
        }

        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
                                
    SQLSelect*                  SQLSelect::where(const std::string& condition)
    { 
        _queryStream << " from " << _currentTable._tableName << " where " << (condition != "" ? condition : "");

        return this;
    }
    SQLSelect*                  SQLSelect::limit(std::uint32_t limit, std::uint32_t offset)
    {
        _queryStream << " from " << _currentTable._tableName << " limit " << limit << " offset " << offset;

        return this;
    }
    SQLSelect*                  SQLSelect::orderBy(const std::initializer_list<std::string>& columnList)
    {
        _queryStream << " from " << _currentTable._tableName << " orderby ";
        for (auto& column : columnList)
        {
            _queryStream << column;
            _queryStream << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
                                
    SQLSelect*                  SQLSelect::And(const std::string& condition)
    {
        if (*std::find(std::begin(_queryStream.str()), std::end(_queryStream.str()), "where") == std::string::npos)
        {
            throw Utility::OperationDBException("First you need to choose column!", __FILE__, __LINE__);
        }
        
        _queryStream << " and " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Or(const std::string& condition)
    {
        if (*std::find(std::begin(_queryStream.str()), std::end(_queryStream.str()), "where") == std::string::npos)
        {
            throw Utility::OperationDBException("First you need to choose column!", __FILE__, __LINE__);
        }
        
        _queryStream << " or " << condition;

        return this;
    }
    SQLSelect*                  SQLSelect::Not(const std::string& condition)
    {
        if (*std::find(std::begin(_queryStream.str()), std::end(_queryStream.str()), "where") == std::string::npos)
        {
            throw Utility::OperationDBException("First you need to choose column!", __FILE__, __LINE__);
        }
        
        _queryStream << " not " << (condition == "" ? "" : condition);

        return this;
    }


    template<typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const DataType&...data)
    {


        return this;
    }
    template<typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const std::tuple<DataType...>& dataList)
    {


        return this;
    }
    template<typename ColumnType ,typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const std::pair<ColumnType, DataType>& ...columnData)
    {


        return this;
    }
    template<typename ColumnType ,typename ...DataType>
    SQLInsert*                  SQLInsert::fields(const std::tuple<std::pair<ColumnType, DataType> ...>& columnDataList)
    {


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
    //[[nodiscard]] Table* Table::insert(void) 
	//{
    //    m_choosedOp = SQLStatement::INSERT;
    //
    //    m_query << "insert into " << m_tableName << " ";
    //
	//	return this;
	//}
    //[[nodiscard]] Table* Table::update(void) 
	//{ 
    //    m_choosedOp = SQLStatement::UPDATE;
    //
    //    m_query << "update " << m_tableName << " ";
    //
	//	return this;
	//}
    //[[nodiscard]] Table* Table::drop(void) 
	//{ 	
    //    m_choosedOp = SQLStatement::DELETE;
    //
    //    m_query << "delete from " << m_tableName << " ";
    //
	//	return this; 
	//}
    //
    //template<typename ...Args>
    //[[nodiscard]] Table* Table::columns(const std::pair<std::string, Args>&... columnList)
	//{
    //    constexpr std::size_t numberOfArgs = (sizeof(Args) + ...);
    //
    //    static_assert(std::is_standard_layout<decltype(columnList.second)>::value, "Data is too complex!");
    //
    //    switch (m_choosedOp)
    //    { 
    //        case SQLStatement::UNKNOWN:
    //        {
    //            throw Utility::EncorrectOperationDBException(
    //                "The startement wasn't choosed!", __FILE__, __LINE__);
    //        }
    //        break;
    //        case SQLStatement::INSERT:
    //        {
    //            m_query << "(";
    //            for (std::size_t i = 0; i < numberOfArgs; i++)
    //            {
    //                m_query << columnList.first;
    //                m_query << (i < numberOfArgs - 1 ? ", " : "");
    //            }
    //            m_query << ") values(";
    //            for (std::size_t i = 0; i < numberOfArgs; i++)
    //            {
    //                m_query << columnList.first;
    //                m_query << (i < numberOfArgs - 1 ? ", " : "")
    //            }
    //            m_query << ")";
    //        }
    //        break;
    //        case SQLStatement::UPDATE:
    //        {
    //            for (std::size_t i = 0; i < numberOfArgs; i++)
    //            {
    //                m_query << "set " << columnList.first << " = " << columnList.second;
    //
    //                m_query << (i < numberOfArgs - 1 ? ", " : " ");
    //            }
    //        }
    //        break;
    //        default: 
    //        {
    //            throw Utility::EncorrectOperationDBException(
    //                "Incorrect startement!", __FILE__, __LINE__);
    //        }
    //        break;
    //    }
    //
	//	return this;
	//}
    //[[nodiscard]] Table* Table::columns(const std::initializer_list<std::string>& columnList)
    //{
    //    switch (m_choosedOp)
    //    {
    //        case SQLStatement::UNKNOWN:
    //        {
    //            throw Utility::EncorrectOperationDBException(
    //                "The startement wasn't choosed!", __FILE__, __LINE__);
    //        }
    //        break;
    //        case SQLStatement::SELECT:
    //        {
    //            for (auto& column : columnList) 
    //                m_query << column << ", ";
    //
    //            m_query << "from " << m_tableName << " ";
    //        }
    //        break;
    //        case SQLStatement::INSERT:
    //        {
    //            m_query << "values(";
    //            std::size_t i = 0;
    //            for (auto& column : columnList)
    //            {
    //                m_query << column;
    //                m_query << (column != *(columnList.end() - 1) ? ", " : " ");
    //            }
    //            m_query << ")";
    //        }
    //        break;
    //        default:
    //        {
    //            throw Utility::EncorrectOperationDBException(
    //                "Incorrect startement!", __FILE__, __LINE__);
    //        }
    //        break;
    //    }
    //
    //    return this;
    //}
    //
    //[[nodiscard]] Table* Table::where(const std::string& condition)
    //{  
    //    m_query << " where " << condition;
    //
    //    return this;
    //}
    //[[nodiscard]] Table* Table::orderBy(const std::initializer_list<std::string>& columnList) 
    //{ 
    //    for (auto& column : columnList)
    //    {
    //        m_query << " order by " << column;
    //        m_query << (column != *(columnList.end() - 1) ? ", " : "");
    //    }
    //
    //    return this;
    //}
    //[[nodiscard]] Table* Table::limit(std::uint32_t limit, std::uint32_t offset)
    //{ 
    //    m_query << " limit " << limit << " offset " << offset;
    //
    //    return this;
    //}
    //
    //void Table::rollback(void) 
    //{ 
    //    m_query.str("");
    //    m_query.clear();
    //}
    //[[nodiscard]] std::optional<pqxx::result> Table::execute(void)
    //{
    //    auto result = mp_postgre->query(m_query.str());
    //
    //    if (result.has_value())
    //    {
    //        return result.value();
    //    }
    // 
    //    return std::nullopt;
    //}
}
