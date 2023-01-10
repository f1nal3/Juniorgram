#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "MockIRepositoryManager.hpp"

namespace MockDataAccess
{
class MockRepositoryManager : public MockIRepositoryManager
{
public:
using MockDataAccess::MockIRepositoryManager::MockIRepositoryManager;

    MockRepositoryManager() = default;
    ~MockRepositoryManager() = default;
};
}  // namespace MockDataAccess
