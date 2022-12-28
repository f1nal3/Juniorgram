#include <iostream>
#include <stdexcept>

#include "ServerBuilder.hpp"
#include "ArgParser.hpp"

using Server::Builder::ServerBuilder;

int main(int argc, const char** argv)
{
    ArgParser parser(argc, argv);
    auto server = ServerBuilder()
                                 .setValue(parser.getPair("--port"))
                                 .setValue(parser.getPair("--dbport"))
                                 .setValue(parser.getPair("--hostaddr"))
                                 .setValue(parser.getPair("--dbname"))
                                 .setValue(parser.getPair("--user"))
                                 .setValue(parser.getPair("--password"))
                                 .make();

    server->start();

    while (true)
    {
        server->update();
    }
    return 0;
}
