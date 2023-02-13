#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerPingRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient,
            MessageType::ServerPing, MessageBody::ExpensiveBody));
    }
    testServerUpdating(testServer);
}

TEST_CASE("TestServerErrorDefaultRequest [success case]")
{
    TestClient mockClient;
    auto       testServer = makeTestServer();
    constexpr auto failedType = int16_t(666);

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, 
            MessageType(failedType), MessageBody::PoorBody));
    }
    testServerUpdating(testServer);
}
