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
    ~ServerBuilder() = default;

    explicit ServerBuilder(const SettingsManager& settingsManager)
        : _settingsManager(settingsManager)
    {}

    std::unique_ptr<Server> makeServer()
    {
        std::unique_ptr<Server> ptr(make());
        return ptr;
    }

private:
    Server* make() noexcept
    {
        auto server = new Server();

        if (!_repository)
        {
            auto repoManager = std::make_unique<DataAccess::PostgreRepositoryManager>(
                DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(_settingsManager.GetConnectionOptions()));
            server->initRepository(std::move(repoManager));
        }
        else
        {
            server->initRepository(std::move(_repository));
        }

        uint16_t port = static_cast<uint16_t>(std::stoi(_settingsManager.GetServerPort()));
        server->initConnection(port);

        return server;
    }

    SettingsManager     _settingsManager;
    RepoManagerUPtr     _repository;
};
}  /// namespace Server::Builder
