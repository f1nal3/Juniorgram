#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "MockRepositories.hpp"

namespace MockObject
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
        repo->registerRepository<DataAccess::IChannelsRepository,      MockChannelsRepository>();
        repo->registerRepository<DataAccess::ILoginRepository,         MockLoginRepository>();
        repo->registerRepository<DataAccess::IMessagesRepository,      MockMessagesRepository>();
        repo->registerRepository<DataAccess::IRegisterRepository,      MockRegisterRepository>();
        repo->registerRepository<DataAccess::IRepliesRepository,       MockRepliesRepository>();
        repo->registerRepository<DataAccess::IDirectMessageRepository, MockDirectMessageRepository>();
        IRepositoryManager::init(std::move(repo));
    }
};
}  // namespace MockObject
