#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("TestServerÑhannelÑreatingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelCreateRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelÑreatingFailedRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelCreateRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelListRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelListRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelSubscribingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelSubscribeRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelSubscribingFailedRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ChannelSubscribeRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelSubscriptionListRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelSubscriptionListRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelLeavingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelLeaveRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelLeavingFailedRequest [success case]")
{
    Client      mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelLeaveRequest, PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelDeletingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelDeleteRequest, StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerÑhannelDeletingFailedRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelDeleteRequest, PoorBody));
    }
    testServerUpdating(testServer);
}
