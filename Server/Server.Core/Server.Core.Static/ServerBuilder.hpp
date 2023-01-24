#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "Server.hpp"

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

    inline ServerBuilder& setValue(std::pair<std::string, std::string> keyValue)
    {
        _arguments.insert(keyValue);
        return *this;
    }

    inline ServerBuilder& setValue(DataAccess::IRepositoryManager* repoManager)
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
    Server* make()
    {
        // form a string with the configuration for connecting to the database
        std::string dbOptions = "";
        for (const auto& [beginArg, endArg] : _arguments)
        {
            if (beginArg != "--serverport")
            {
                for (size_t i = 0; i < beginArg.length(); ++i)
                {
                    if (beginArg[i] != '-')
                    {
                        dbOptions += beginArg[i];
                    }
                }
                dbOptions += "=";
                dbOptions += endArg;
                dbOptions += " ";
            }
        }
        
        auto server = new Server();

        if (!_repository)
        {
            auto repoManager = std::make_unique<DataAccess::PostgreRepositoryManager>(
                DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(dbOptions));
            server->initRepository(std::move(repoManager));
        }
        else
        {
            server->initRepository(std::move(_repository));
        }

        uint16_t port = std::stoi(_arguments["--serverport"]);
        server->initConnection(port);

        return server;
    }

    std::map<std::string, std::string>  _arguments;
    RepoManagerPtr                      _repository;
};
}  // namespace Server::Builder
