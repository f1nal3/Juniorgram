#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "MockIRepositoryManager.hpp"

namespace MockDataAccess
{
using MockDataAccess::MockIRepositoryManager;

class MockRepositoryManager : public MockIRepositoryManager
{
public:
    MockRepositoryManager() = default;

    explicit MockRepositoryManager(const std::shared_ptr<DataAccess::IAdapter>& repositoryContainer) 
        : MockIRepositoryManager(repositoryContainer)
    {
    }

    ~MockRepositoryManager() = default;
};
}  // namespace MockDataAccess
