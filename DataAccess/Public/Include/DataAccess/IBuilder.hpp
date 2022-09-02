#pragma once

#include <Utility/SQLUtility.hpp>
#include <Utility/Utility.hpp>

#include "SQLStatements.hpp"

namespace DataAccess
{
/**
* @class IBuilder
* @brief IBuilder interface.
*/
class IBuilder
{
public:
    virtual [[nodiscard]] std::shared_ptr<IAdapter> getAdapter() const noexcept = 0;

    virtual [[nodiscard]] Utility::DatabaseType getDatabaseType() const noexcept = 0;

    virtual [[nodiscard]] const std::string& getCurrentTableName() const noexcept = 0;
	
    virtual ~IBuilder() = default;
};
}  // namespace DataAccess
