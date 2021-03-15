#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Network/Server.hpp"
#include "ArgumentParser.h"

int main(int argc, const char** argv)
{
    try
    {
        ArgumentParser parser(argc, argv);

        network::Server server(parser.getPort());
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
