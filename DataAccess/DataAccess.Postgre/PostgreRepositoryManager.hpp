#pragma once
#include <DataAccess/AbstractRepositoryManager.hpp>

#include "PostgreRepositoryContainer.hpp"
#include "PostgreRepositories.hpp"

namespace DataAccess
{
/**
 * @class   PostgreRepositoryManager
 * @brief   The specific(Postgre) instance of IRepositoryManager
 * @details Controls push to queue and further processing of requests (have own thread for it).
 */
class PostgreRepositoryManager: public IRepositoryManager
{
public:
    explicit PostgreRepositoryManager(const std::shared_ptr<IAdapter>& repositoryContainer)
    {
        _repositories = std::make_unique<AbstractRepositoryContainer>(repositoryContainer);
    }

    void registerRepositories() const override
    {
        _repositories->registerRepository<IChannelsRepository, ChannelsRepository>();
        _repositories->registerRepository<ILoginRepository, LoginRepository>();
        _repositories->registerRepository<IMessagesRepository, MessagesRepository>();
        _repositories->registerRepository<IRegisterRepository, RegisterRepository>();
        _repositories->registerRepository<IRepliesRepository, RepliesRepository>();
        _repositories->registerRepository<IDirectMessageRepository, DirectMessageRepository>();
    }
};
}  // namespace DataAccess
