#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "Server.hpp"
#include "SettingsManager.hpp"

namespace Server::Builder
{
/**
 * @brief Class builder of Server.
 * @details Class based on builder pattern for creation instances of @link Server @endlink.
 */
class ServerBuilder
{
public:
    ServerBuilder()  = default;
    explicit ServerBuilder(std::unique_ptr<SettingsManager> settingsManager) : _settingsManager(std::move(settingsManager)) {}
    
    ~ServerBuilder() = default;

    /**
    * @brief Sets up a specific RepositoryManager
    * @details Allows to set a specific RepositoryManager
    *          Note: Changes the owner of the argument resource
    * @warning For now, this method is only used for MockTest! This may be changed in the future.
    */
    ServerBuilder& setRepoManager(std::unique_ptr<DataAccess::IRepositoryManager> repoManager)
    {
        _repository = std::move(repoManager);
        return *this;
    }

    std::unique_ptr<Server> makeServer()
    {
        std::unique_ptr<Server> ptr(make());
        return ptr;
    }

private:
    Server* make() noexcept
    {
        auto server = std::unique_ptr<Server>(new Server());

        if (!_repository)
        {
            _repository = std::make_unique<DataAccess::PostgreRepositoryManager>
                (DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(_settingsManager->getConnectionOptions()));
        }
       
        server->initRepository(std::move(_repository));

        auto port = _settingsManager->getServerPort();
        server->initConnection(port);

        return server.release();
    }

    std::unique_ptr<SettingsManager>    _settingsManager;
    RepoManagerUPtr                     _repository;
};
}  /// namespace Server::Builder
