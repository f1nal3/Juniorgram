#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("TestServerStartUp [success case]")
{
    Client     mockClient;
    auto       testServer = makeTestServer();

    CHECK_NOTHROW(testServer->start());
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        bindOfSendingMessage(mockClient, MessageType::ServerAccept);
    }
    REQUIRE_NOTHROW(testServerUpdating(testServer));
}

suppressWarning(4244, "-Wconversion")
suppressWarning(4242, "-Wconversion")
TEST_CASE("TestServerFailStartUpWithBadPort [failed case]")
{   
    TestServer testServer;
    REQUIRE_THROWS_AS(testServer = makeTestBadServer(), std::exception);
}
restoreWarning
restoreWarning
