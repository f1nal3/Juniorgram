#include <ArgumentParser.hpp>
#include <Server.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, const char** argv)
{
    try
    {
        ArgumentParser parser(argc, argv);

        Server::Server server(parser.getPort());
        server.registerRepositories();
        server.start();

        while (true)
        {
            server.update();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
