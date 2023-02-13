#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerChannelCreatingFailedRequest [success case]")
{
    TestClient  mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelCreateRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelSubscribingFailedRequest [success case]")
{
    TestClient  mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelSubscribeRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelLeavingFailedRequest [success case]")
{
    TestClient  mockClient;
    auto        testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelLeaveRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerChannelDeletingFailedRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelDeleteRequest, MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
