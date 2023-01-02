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
 * @brief Class builder of Server
 * @details Class based on builder pattern for creation instances of @link Server @endlink
 * @todo Think of a more elegant repository initialization interface
 */
class ServerBuilder {
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

    Server* makeServer() noexcept
    {
        auto obj = new Server();

        // form a string with the configuration for connecting to the database
        std::string options = "";
        for (const auto& arg : _arguments)
        {
            if (arg.first != "--port")
            {
                for (size_t i = 0; i < arg.first.length(); ++i)
                {
                    if (arg.first[i] != '-')
                    {
                        options += arg.first[i];
                    }
                }
                options += "=";
                options += arg.second;
                options += " ";
            }
        }

        const char* dbOptions = options.c_str();
        using DataAccess::IAdapter;
        if (!_repository)
        {
            auto up_repo = std::make_unique<DataAccess::PostgreRepositoryManager>(IAdapter::getInstance<DataAccess::PostgreAdapter>(dbOptions));
            obj->initRepository(std::move(up_repo));
        }
        else
        {
            obj->initRepository(std::move(_repository));
        }

        // initialize host port
        uint16_t port = std::stoi(_arguments["--port"]);
        obj->initConnection(port);

        return obj;
    }

    std::unique_ptr<Server> makeServerRAII() noexcept
    {
        std::unique_ptr<Server> uptr(makeServer());
        return uptr;
    }

private:
    std::map<std::string, std::string>  _arguments;
    RepoManager_uptr                    _repository;
};
}  // namespace Server::Builder
