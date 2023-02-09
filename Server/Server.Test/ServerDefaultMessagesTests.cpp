#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;
using TestUtility::TypeOfMessageBody;

TEST_CASE("TestServerPingRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ServerPing, TypeOfMessageBody::StrongBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerErrorDefaultRequest [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();
    constexpr auto failedType = int16_t(666);

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType(failedType), TypeOfMessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}