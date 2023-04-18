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
            //std::string str         = "dbname=juniorgram hostaddr=127.0.0.1 password=BrayanPGSQLPass@00 port=5432 user=postgres";
            std::string str         = "dbname=mockdb hostaddr=127.0.0.1 password=tester port=5432 user=tester";
            auto        repoManager = std::make_unique<DataAccess::PostgreRepositoryManager>(
                //DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(_settingsManager.GetConnectionOptions()));
                DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(str));
            /** /
            _settings = {
            _serverPort = "65001"
            _hostAddress = "127.0.0.1"
            _dbName = "mockdb"
            _dbPort = "5432"
            _dbUser = "tester"
            _dbPassword = "tester"
            }

            _settings = {
            _serverPort = "65001"
            _hostAddress = "127.0.0.1"
            _dbName = "juniorgram"
            _dbPort = "5432"
            _dbUser = "postgres"
            _dbPassword = "BrayanPGSQLPass@00"
            }
            /**/
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
