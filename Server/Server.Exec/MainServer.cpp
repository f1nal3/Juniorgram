#include <iostream>
#include <stdexcept>

#include "ServerBuilder.hpp"
#include "ArgParser.hpp"

using Server::Builder::ServerBuilder;

int main(int argc, const char** argv)
{
    ArgParser parser(argc, argv);
    auto serverBuild = ServerBuilder()
                                 .setValue(parser.getPair("--serverport"))
                                 .setValue(parser.getPair("--dbname"))
                                 .setValue(parser.getPair("--hostaddr"))
                                 .setValue(parser.getPair("--port"))
                                 .setValue(parser.getPair("--user"))
                                 .setValue(parser.getPair("--password"))
                                 /*.setValue("managerName")*/
                                 .serverInit();

    serverBuild->start();

    while (true)
    {
        serverBuild->update();
    }
    return 0;
}
