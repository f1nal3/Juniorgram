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
 * @brief IQuery class.
 */

class IQuery
{
public:
    virtual [[nodiscard]] std::shared_ptr<IAdapter> getAdapter() const noexcept = 0;

    virtual [[nodiscard]] Utility::DatabaseType getDatabaseType() const noexcept = 0;

    virtual [[nodiscard]] const std::string& getCurrentTableName() const noexcept = 0;

    virtual ~IQuery() = default;
};
}  // namespace DataAccess
