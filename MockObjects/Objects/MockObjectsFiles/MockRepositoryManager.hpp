#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "MockRepositories.hpp"

namespace MockDataAccess
{
/**
* @brief class MockRepositoryManager.
* @details The specific(Postgre) instance of IRepositoryManager. /
*          Controls push to queue and further processing of requests (have own thread for it).
*/
class MockRepositoryManager : public DataAccess::IRepositoryManager
{
public:
    explicit MockRepositoryManager(const std::shared_ptr<DataAccess::IAdapter> repositoryContainer)
    {
        auto repo = std::make_unique<DataAccess::AbstractRepositoryContainer>(repositoryContainer);
        repo->registerRepository<DataAccess::IChannelsRepository, MockRepositories::testChannelsRepository>();
        repo->registerRepository<DataAccess::ILoginRepository, MockRepositories::testLoginRepository>();
        repo->registerRepository<DataAccess::IMessagesRepository, MockRepositories::testMessagesRepository>();
        repo->registerRepository<DataAccess::IRegisterRepository, MockRepositories::testRegisterRepository>();
        repo->registerRepository<DataAccess::IRepliesRepository, MockRepositories::testRepliesRepository>();
        repo->registerRepository<DataAccess::IDirectMessageRepository, MockRepositories::testDirectMessageRepository>();
        IRepositoryManager::init(std::move(repo));
    }
};
}  // namespace MockDataAccess
