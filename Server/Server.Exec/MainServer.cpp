#include <iostream>
#include <stdexcept>

#include <Server.hpp>
#include <ArgumentParser.hpp>

int main(int argc, const char** argv)
{
    using Base::Logger::LogLevel;
    using Base::Logger::FileLogger;

    uint16_t port;

    try
    {
        ArgumentParser parser(argc, argv);
        port = parser.getPort();
    }
    catch (const std::runtime_error& e)
    {
        FileLogger::getInstance().log
        (
            e.what(), 
            LogLevel::ERR
        );
    }

    Server::Server server(port);
    server.start();

    while (true)
    {
        server.update();
    }
    return 0;
}
