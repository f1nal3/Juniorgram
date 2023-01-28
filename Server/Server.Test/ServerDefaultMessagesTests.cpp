#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow of checking server ping")
{
    Client     mockClient;
    ArgParser  parser(configArgc, configArgv);
    TestServer server = ServerBuilder()
                            .setValue(parser.getPair("--serverport"))
                            .setValue(parser.getPair("--dbname"))
                            .setValue(parser.getPair("--hostaddr"))
                            .setValue(parser.getPair("--port"))
                            .setValue(parser.getPair("--user"))
                            .setValue(parser.getPair("--password"))
                            .setValue(getTestingDatabase().release())
                            .makeServer();
    server->start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ServerPing));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking default request of server side")
{
    Client     mockClient;
    ArgParser  parser(configArgc, configArgv);
    TestServer server = ServerBuilder()
                            .setValue(parser.getPair("--serverport"))
                            .setValue(parser.getPair("--dbname"))
                            .setValue(parser.getPair("--hostaddr"))
                            .setValue(parser.getPair("--port"))
                            .setValue(parser.getPair("--user"))
                            .setValue(parser.getPair("--password"))
                            .setValue(getTestingDatabase().release())
                            .makeServer();
    server->start();

    constexpr auto failedType         = int16_t(666);
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType(failedType)));
    }
    testServerUpdating(server);
}
