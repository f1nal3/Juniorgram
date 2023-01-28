#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow of checking server ping")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ServerPing));
    }
    testServerUpdating(testServer);
}

TEST_CASE("Workflow of checking default request of server side")
{
    Client     mockClient;
    auto       testServer = makeTestServer();
    constexpr auto failedType = int16_t(666);

    testServer->start();
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType(failedType)));
    }
    testServerUpdating(testServer);
}
