#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow startup server")
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
TEST_CASE("Workflow fail startup of server with bad port")
{    
    auto           testServer  = makeTestBadServer();
    CHECK_NOTHROW(testServer->start());
}
restoreWarning
restoreWarning
