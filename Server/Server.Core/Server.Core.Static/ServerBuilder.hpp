#pragma once

#include <string>
#include <map>
#include <memory>

#include "Server.hpp"


namespace Server::Builder
{
    
    class ServerBuilder
    {
    public:
        ServerBuilder() = default;
        ~ServerBuilder() = default;
    
        template <typename T>
        inline ServerBuilder& setValue(std::string_view key, T value)
        {
            _arguments.insert({key, value});
            return *this;
        }

        template <typename T>
        inline ServerBuilder& setValue(std::pair<std::string_view, T> keyValue)
        {
            _arguments.insert(keyValue);
            return *this;
        }

        Server* ServerBuilder::make()
        {
            auto obj = new Server();

            auto port       = convert<int>("--port");
            auto dbOptions  = convert<const char*>("--db_config");
            
            auto up_repo = std::make_unique<PostgreRepositoryManager>(DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(dbOptions));

            obj->initRepository(std::move(up_repo));
            obj->initConnection(static_cast<uint16_t>(port));
            return obj;
        }

    private:
        template <typename T>
        inline T convert(std::string_view key)
        {
            return std::any_cast<T>(_arguments[key]);
        }

    private:
        std::map<std::string_view, std::any> _arguments;
    };
}
