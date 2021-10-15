#pragma once
#include <../../Public/Include/DataAccess/AbstractRepositoryContainer.hpp>
#include <utility>

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

}  // namespace DataAccess
