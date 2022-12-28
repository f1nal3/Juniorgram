#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Check all messages from server side")
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

TEST_CASE("Check message history request of server")
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

TEST_CASE("Check reply message history request of server")
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


 TEST_CASE("Check direct message create request from server side")
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

TEST_CASE("Check message store request from server side")
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

TEST_CASE("Check reply message store request from server side")
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

TEST_CASE("Check message reaction request from server side")
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

TEST_CASE("Check message edit from server side")
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

TEST_CASE("Check message delete request from server side")
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
