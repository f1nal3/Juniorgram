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
                                 .setValue(parser.getPair("--hostaddr"))
                                 .setValue(parser.getPair("--dbport"))
                                 .setValue(parser.getPair("--dbname"))
                                 .setValue(parser.getPair("--dbuser"))
                                 .setValue(parser.getPair("--dbpassword"))
                                // .setValue("repo", new DataAccess::PostgreRepositoryManager())
                                 .makeServerRAII();

    server->start();

    while (true)
    {
        server->update();
    }
    return 0;
}
