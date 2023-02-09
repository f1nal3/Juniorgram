#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("TestServerStartingUp [success case]")
{
    Client     mockClient;
    TestServer testServer;

    CHECK_NOTHROW(testServer = makeTestServer());
    REQUIRE_NOTHROW(testServer->start());
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        bindOfSendingMessage(mockClient, MessageType::ServerAccept, StrongBody);
    }
    REQUIRE_NOTHROW(testServerUpdating(testServer));
}

TEST_CASE("TestServerFailStartingUpWithBadPort [failed case]")
{   
    TestServer testServer;
    CHECK_FALSE(makeTestBadServer(testServer));
}
