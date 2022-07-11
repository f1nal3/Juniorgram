
#include <iostream>
#include <stdexcept>

#include "ServerBuilder.hpp"
#include "ArgParser.hpp"

using Server::Builder::ServerBuilder;

// "dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432"
// "hostaddr=127.0.0.1 port=5432 dbname=testdb user=postgres"

int main(int argc, const char** argv)
{
    ArgParser parser(argc, argv);
    auto server = ServerBuilder()
                                .setValue(parser.getPair<int>("--port"))
                                .setValue("--db_config", "hostaddr=127.0.0.1 port=5432 dbname=juniorgram user=postgres password=1111")
                                .make();
    server->start();

    while (true)
    {
        server->update();
    }
    return 0;
}