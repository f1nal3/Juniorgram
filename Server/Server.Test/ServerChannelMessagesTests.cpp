#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow of checking channel create request of server")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelCreateRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking channel list request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelListRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking channel subscribe request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelSubscribeRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking channel subscription list request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelSubscriptionListRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking channel leave request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelLeaveRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking channel delete request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelDeleteRequest));
    }
    testServerUpdating(server);
}
