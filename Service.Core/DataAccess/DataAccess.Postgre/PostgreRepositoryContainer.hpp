#pragma once

#include "DataAccess/AbstractRepositoryContainer.hpp"
#include "PostgreAdapter.hpp"

namespace DataAccess
{
class PostgreRepositoryContainer : public AbstractRepositoryContainer
{
public:
    PostgreRepositoryContainer(std::shared_ptr<IAdapter> adapter) : AbstractRepositoryContainer(adapter) {}
};
}  // namespace DataAccess
