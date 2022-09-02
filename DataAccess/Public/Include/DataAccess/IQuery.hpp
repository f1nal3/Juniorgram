#pragma once

#include <DataAccess/IAdapter.hpp>

#include <Utility/Exception.hpp>
#include <Utility/SQLUtility.hpp>
#include <Utility/Utility.hpp>

#include "SQLStatements.hpp"

namespace DataAccess
{
/**
* @class IQuery
* @brief IQuery interface.
*/
template <typename ResultType>
class IQuery
{
public:
    virtual SQLSelect<ResultType>* Select() = 0;

    virtual SQLInsert<ResultType>* Insert() = 0;

    virtual SQLUpdate<ResultType>* Update() = 0;

    virtual SQLDelete<ResultType>* Delete() = 0;

    virtual void changeTable(const char* newTableName) noexcept = 0;

    virtual void changeTable(const std::string& newTableName) noexcept = 0;

    virtual void clearStatement() = 0;

    virtual ~IQuery() = default;
};
}  // namespace DataAccess
