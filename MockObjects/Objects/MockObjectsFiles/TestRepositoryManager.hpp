#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "TestIRepositoryManager.hpp"

namespace TestDataAccess
{
class TestIRepositoryManager;

class TestRepositoryManager : public TestIRepositoryManager
{
public:
    TestRepositoryManager() = default;

    explicit TestRepositoryManager(const std::shared_ptr<DataAccess::IAdapter>& repositoryContainer) 
        : TestIRepositoryManager(repositoryContainer)
    {
    }

    ~TestRepositoryManager() {}
};
}  // namespace TestDataAccess
