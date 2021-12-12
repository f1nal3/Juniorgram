#include <iostream>
#include <stdexcept>

#include <Server.hpp>
#include <ArgumentParser.hpp>

int main(int argc, const char** argv)
{
    try
    {
        ArgumentParser parser(argc, argv);

        Server::Server server(parser.getPort());
        server.start();

        while (true)
        {
            server.update();
        }
    }
    catch (const std::exception& e)
    {
        Base::Logger::FileLogger::getInstance().log(e.what(), Base::Logger::LogLevel::ERROR);
    }

    return 0;
}
