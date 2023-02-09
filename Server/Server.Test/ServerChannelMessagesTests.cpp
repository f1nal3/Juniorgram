#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("TestServer�hannel�reatingRequest [success case]")
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

TEST_CASE("TestServer�hannel�reatingFailedRequest [success case]")
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

TEST_CASE("TestServer�hannelListRequest [success case]")
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

TEST_CASE("TestServer�hannelSubscribingRequest [success case]")
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

TEST_CASE("TestServer�hannelSubscribingFailedRequest [success case]")
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

TEST_CASE("TestServer�hannelSubscriptionListRequest [success case]")
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

TEST_CASE("TestServer�hannelLeavingRequest [success case]")
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

TEST_CASE("TestServer�hannelLeavingFailedRequest [success case]")
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

TEST_CASE("TestServer�hannelDeletingRequest [success case]")
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

TEST_CASE("TestServer�hannelDeletingFailedRequest [success case]")
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
