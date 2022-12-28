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
 
        inline ServerBuilder& setValue(std::pair<std::string, std::string> keyValue)
        {
            _arguments.insert(keyValue);
            return *this;
        }

        Server* ServerBuilder::make()
        {
            auto obj = new Server();
                                   
            std::string options = "";
            for (const auto& arg : _arguments)
            {
                if (arg.first != "--port")
                {
                    for (int i = 0; i < arg.first.length(); ++i)
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

            int port = std::stoi(_arguments["--port"]);
            const char* dbOptions = options.c_str();           
                                               
            auto up_repo = std::make_unique<PostgreRepositoryManager>(DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(dbOptions));

            obj->initRepository(std::move(up_repo));
            obj->initConnection(static_cast<uint16_t>(port));
            return obj;
        }
   
    private:
        std::map<std::string, std::string> _arguments;
    };
}
