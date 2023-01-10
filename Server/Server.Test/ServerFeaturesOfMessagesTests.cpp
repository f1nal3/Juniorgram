#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Workflow of checking all messages from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageAll));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Workflow of checking message history request of server")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageHistoryRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Workflow of checking reply message history request of server")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ReplyHistoryRequest));
    }
    testServerUpdating(serverTest);
}


 TEST_CASE("Workflow of checking direct message create request from server side")
  {
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::DirectMessageCreateRequest));
    }
    testServerUpdating(serverTest);
   }

TEST_CASE("Workflow of checking message store request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageStoreRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Workflow of checking reply message store request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ReplyStoreRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Workflow of checking message reaction request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageReactionRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Workflow of checking message edit from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageEditRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Workflow of checking message delete request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::MessageDeleteRequest));
    }
    testServerUpdating(serverTest);
}
