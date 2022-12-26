#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "TestIRepositoryManager.hpp"

namespace TestDataAccess
{
using TestDataAccess::TestIRepositoryManager;

class TestRepositoryManager : public TestIRepositoryManager
{
public:
    TestRepositoryManager() = default;

    explicit TestRepositoryManager(const std::shared_ptr<DataAccess::IAdapter>& repositoryContainer) 
        : TestIRepositoryManager(repositoryContainer)
    {
    }

   virtual ~TestRepositoryManager() = default;
};
}  // namespace TestDataAccess
