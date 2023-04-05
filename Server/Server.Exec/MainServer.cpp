#include <iostream>
#include <stdexcept>

#include "ServerBuilder.hpp"
#include "ArgParser.hpp"

using Server::Builder::ServerBuilder;

int main(int argc, const char** argv)
{
    auto server = ServerBuilder(SettingsManager(Settings(ArgParser(argc, argv)))).makeServer();
    
    server->start();

    while (true)
    {
        server->update();
    }
    return 0;
}
