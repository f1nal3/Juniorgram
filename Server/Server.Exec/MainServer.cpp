#include <iostream>
#include <stdexcept>

#include <Server.hpp>
#include <ArgumentParser.hpp>

int main(int argc, const char** argv)
{
    uint16_t port;
    try
    {
        ArgumentParser parser(argc, argv);
        port = parser.getPort();
    }
    catch (const std::runtime_error& e)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            e.what(), 
            Base::Logger::LogLevel::ERR
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
