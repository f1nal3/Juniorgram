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
    PostgreRepositoryManager(){}

    void GetConnectionOptions(std::string options)
    {
        auto container = IAdapter::getInstance<DataAccess::PostgreAdapter>(options);
        auto repo      = std::make_unique<AbstractRepositoryContainer>(container);
        repo->registerRepository<IChannelsRepository, ChannelsRepository>();
        repo->registerRepository<ILoginRepository, LoginRepository>();
        repo->registerRepository<IMessagesRepository, MessagesRepository>();
        repo->registerRepository<IRegisterRepository, RegisterRepository>();
        repo->registerRepository<IRepliesRepository, RepliesRepository>();
        repo->registerRepository<IDirectMessageRepository, DirectMessageRepository>();
    
        IRepositoryManager::init(std::move(repo));
    }
};
}  // namespace DataAccess
