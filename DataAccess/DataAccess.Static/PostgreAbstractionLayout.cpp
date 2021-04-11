#include "PostgreAbstractionLayout.hpp"

namespace DataAccess
{
	Table* Table::select(void) 
	{
        m_choosedOp = Operation::SELECT;

		m_query << "select ";

		return this;
	}
	Table* Table::insert(void) 
	{
        m_choosedOp = Operation::INSERT;

        m_query << "insert into " << m_tableName << " ";

		return this;
	}
    Table* Table::update(void) 
	{ 
        m_choosedOp = Operation::UPDATE;

        m_query << "update " << m_tableName << " ";

		return this;
	}
    Table* Table::drop(void) 
	{ 	
        m_choosedOp = Operation::DELETE;

        m_query << "delete from " << m_tableName << " ";

		return this; 
	}

    template<typename ...Args>
    Table* Table::columns(const std::pair<std::string, Args>&... columnList)
	{
        constexpr std::size_t numberOfArgs = (sizeof(Args) + ...);

        static_assert(std::is_standard_layout<decltype(columnList.second)>::value, "Data is too complex!");

        switch (m_choosedOp)
        { 
            case Operation::NOTHING:
            {
                throw Utility::EncorrectOperationDBException(
                    "The operation wasn't choosed!", __FILE__, __LINE__);
            }
            break;
            case Operation::INSERT:
            {
                m_query << "(";
                for (std::size_t i = 0; i < numberOfArgs; i++)
                {
                    m_query << columnList.first;
                    m_query << (i < numberOfArgs - 1 ? ", " : "");
                }
                m_query << ") values(";
                for (std::size_t i = 0; i < numberOfArgs; i++)
                {
                    m_query << columnList.first;
                    m_query << (i < numberOfArgs - 1 ? ", " : "")
                }
                m_query << ")";
            }
            break;
            case Operation::UPDATE:
            {
                for (std::size_t i = 0; i < numberOfArgs; i++)
                {
                    m_query << "set " << columnList.first << " = " << columnList.second;

                    m_query << (i < numberOfArgs - 1 ? ", " : " ");
                }
            }
            break;
            default: 
            {
                throw Utility::EncorrectOperationDBException(
                    "Incorrect operation!", __FILE__, __LINE__);
            }
            break;
        }

		return this;
	}
    Table* Table::columns(const std::initializer_list<std::string>& columnList)
    {
        switch (m_choosedOp)
        {
            case Operation::NOTHING:
            {
                throw Utility::EncorrectOperationDBException(
                    "The operation wasn't choosed!", __FILE__, __LINE__);
            }
            break;
            case Operation::SELECT:
            {
                for (auto& column : columnList) 
                    m_query << column << ", ";

                m_query << "from " << m_tableName << " ";
            }
            break;
            case Operation::INSERT:
            {
                m_query << "values(";
                std::size_t i = 0;
                for (auto& column : columnList)
                {
                    m_query << column;
                    m_query << (column != *(columnList.end() - 1) ? ", " : " ");
                }
                m_query << ")";
            }
            break;
            default:
            {
                throw Utility::EncorrectOperationDBException(
                    "Incorrect operation!", __FILE__, __LINE__);
            }
            break;
        }

        return this;
    }

    Table* Table::where(const std::string& condition)
    {  
        if (*std::find(std::begin(m_query.str()), std::end(m_query.str()), "where") ==
            std::string::npos)
        {
            m_query << " where " << condition;
        }
        else
        {
            m_query << " and " << condition;
        }

        return this;
    }
    Table* Table::orderBy(const std::initializer_list<std::string>& columnList) 
    { 
        for (auto& column : columnList)
        {
            m_query << " order by " << column;
            m_query << (column != *(columnList.end() - 1) ? ", " : "");
        }

        return this;
    }
    Table* Table::limit(std::uint32_t limit, std::uint32_t offset)
    { 
        m_query << " limit " << limit << " offset " << offset;

        return this;
    }

    void Table::rollback(void) 
    { 
        m_query.str("");
        m_query.clear();
    }
    std::optional<pqxx::result> Table::execute(void)
    {
        auto result = mp_postgre->query(m_query.str());

        if (result.has_value())
        {
            return result.value();
        }
     
        return std::nullopt;
    }
}
