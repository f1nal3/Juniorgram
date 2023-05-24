#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "MockRepositories.hpp"

namespace MockObject
{
using DataAccess::IRepositoryManager;
using DataAccess::AbstractRepositoryContainer;

/**
* @brief class MockRepositoryManager.
* @details The specific(Postgre) instance of IRepositoryManager. /
*          Controls push to queue and further processing of requests (have own thread for it).
*/
class MockRepositoryManager : public IRepositoryManager
{
public:
    explicit MockRepositoryManager(const std::shared_ptr<IAdapter> repositoryContainer)
    {
        auto repo = std::make_unique<AbstractRepositoryContainer>(repositoryContainer);
        repo->registerRepository<DataAccess::IChannelsRepository<MockModelFiller>,       MockChannelsRepository>();
        repo->registerRepository<ILoginRepository,         MockLoginRepository>();
        repo->registerRepository<IMessagesRepository,      MockMessagesRepository>();
        repo->registerRepository<IRegisterRepository,      MockRegisterRepository>();
        repo->registerRepository<IRepliesRepository,       MockRepliesRepository>();
        repo->registerRepository<IDirectMessageRepository, MockDirectMessageRepository>();
        IRepositoryManager::init(std::move(repo));
    }
};
}  /// namespace MockObject
