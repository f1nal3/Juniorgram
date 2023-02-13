#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerChannelCreatingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelCreateRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelListRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelListRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelSubscribingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelSubscribeRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelSubscriptionListRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelSubscriptionListRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelLeavingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelLeaveRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelDeletingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelDeleteRequest, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}
