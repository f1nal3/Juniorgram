#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Check channel create request of server")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());
      
    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelCreateRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel list request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelListRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel subscribe request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelSubscribeRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel subscription list request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelSubscriptionListRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel leave request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelLeaveRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel delete request from server side")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelDeleteRequest));
    }
    testServerUpdating(serverTest);
}
