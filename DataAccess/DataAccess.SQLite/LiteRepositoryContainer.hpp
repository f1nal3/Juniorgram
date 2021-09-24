#pragma once
#include <AbstarctRepositoryContainer.hpp>

#include "LiteAdapter.hpp"

namespace DataAccess
{
/**
 * @class LiteRepositoryContainer
 * @brief Repository container for SQLite database
 */
class LiteRepositoryContainer : public AbstarctRepositoryContainer
{
public:
    /// Default constructor from adapter
    LiteRepositoryContainer(std::shared_ptr<IAdapter> adapter) : AbstarctRepositoryContainer(adapter) {}
};

}  // namespace DataAccess
