#include <iostream>
#include <stdexcept>

#include "ServerBuilder.hpp"
#include "ArgParser.hpp"

using Server::Builder::ServerBuilder;
using Server::Builder::SettingsManager;

int main(int argc, const char** argv)
{
    ArgParser parser(argc, argv);
    auto      server = ServerBuilder(std::make_unique<SettingsManager>(parser.GetSettings())).makeServer();

    server->start();

    while (true)
    {
        server->update();
    }
    return 0;
}
