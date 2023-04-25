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
    explicit ServerBuilder(SettingsManager settingsManager) : _settingsManager(settingsManager) {}

    ~ServerBuilder() = default;

    /**
    * @brief Sets up a specific RepositoryManager
    * @details Allows to set a specific RepositoryManager
    * @warning This method is to MockTest only! It may disappear in future.
    */
    inline ServerBuilder& SetMockRepo(DataAccess::IRepositoryManager* repoManager)
    {
        _repository.reset(repoManager);
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
                (DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(_settingsManager.GetConnectionOptions()));
        }
       
        server->initRepository(std::move(_repository));

        uint16_t port = static_cast<uint16_t>(stoi(_settingsManager.GetServerPort()));
        server->initConnection(port);

        return server.release();
    }

    SettingsManager     _settingsManager;
    RepoManagerUPtr     _repository;
};
}  /// namespace Server::Builder
