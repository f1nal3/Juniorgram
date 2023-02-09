#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;
using TestUtility::TypeOfMessageBody;

TEST_CASE("TestServerÑhannelÑreatingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType::ChannelCreateRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ChannelCreateRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::ChannelListRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ChannelSubscribeRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ChannelSubscribeRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::ChannelSubscriptionListRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ChannelLeaveRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ChannelLeaveRequest, TypeOfMessageBody::PoorBody));
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
            MessageType::ChannelDeleteRequest, TypeOfMessageBody::StrongBody));
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
            MessageType::ChannelDeleteRequest, TypeOfMessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
