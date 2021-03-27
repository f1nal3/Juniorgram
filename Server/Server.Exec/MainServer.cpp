#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include <Server.hpp>
#include <ArgumentParser.hpp>

int main(int argc, const char** argv)
{

    try
    {
        ArgumentParser parser(argc, argv);

        server::Server server(parser.getPort());
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
