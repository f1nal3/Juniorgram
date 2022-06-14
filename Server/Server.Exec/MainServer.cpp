#include <ArgParser.hpp>
#include <Server.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, const char** argv)
{
    ArgParser parser(argc, argv);
    Server::Server server(ArgParser::getPort());
    server.start();

    while (true)
    {
        server.update();
    }
    return 0;
}