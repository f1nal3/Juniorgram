#pragma once
#include <utility>

#include <DataAccess/AbstractRepositoryContainer.hpp>

#include "LiteAdapter.hpp"
#include "LiteRepositories.hpp"

namespace DataAccess
{
/**
 * @class LiteRepositoryContainer
 * @brief Repository container for SQLite database
 */
class LiteRepositoryContainer : public AbstractRepositoryContainer
{
public:
    /// Default constructor from adapter
    explicit LiteRepositoryContainer(std::shared_ptr<IAdapter> adapter) : AbstractRepositoryContainer(std::move(adapter)) {}
};

}  /// namespace DataAccess
