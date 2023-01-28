#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow of checking registration request of server")
{
    Client      mockClient;
    ArgParser   parser(configArgc, configArgv);
    TestServer  server = ServerBuilder()
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::RegistrationRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking login request of server")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::LoginRequest));
    }
    testServerUpdating(server);
}
