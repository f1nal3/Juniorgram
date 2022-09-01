#pragma once

#include <Utility/SQLUtility.hpp>
#include <Utility/Utility.hpp>

#include "SQLStatements.hpp"

namespace DataAccess
{
/**
* @class IBuilder
* @brief IBuilder class.
*/

template<typename ResultType>
class IBuilder
{
public:
    virtual SQLSelect<ResultType>* Select() = 0;

    virtual SQLInsert<ResultType>* Insert() = 0;

    virtual SQLUpdate<ResultType>* Update() = 0;

    virtual SQLDelete<ResultType>* Delete() = 0;

    virtual void changeTable(const char* newTableName) noexcept = 0;

    virtual void changeTable(const std::string& newTableName) noexcept = 0;

    virtual void clearStatement() = 0;
	
    virtual ~IBuilder() = default;
};
}  // namespace DataAccess
