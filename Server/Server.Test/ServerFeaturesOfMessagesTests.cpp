#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow of checking all messages from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageAll));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking message history request of server")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageHistoryRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking reply message history request of server")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ReplyHistoryRequest));
    }
    testServerUpdating(server);
}


 TEST_CASE("Workflow of checking direct message create request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::DirectMessageCreateRequest));
    }
    testServerUpdating(server);
   }

TEST_CASE("Workflow of checking message store request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageStoreRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking reply message store request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ReplyStoreRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking message reaction request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageReactionRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking message edit from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageEditRequest));
    }
    testServerUpdating(server);
}

TEST_CASE("Workflow of checking message delete request from server side")
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
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageDeleteRequest));
    }
    testServerUpdating(server);
}
