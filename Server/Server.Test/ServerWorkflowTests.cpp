#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerStartingUp [success case]")
{
    TestClient mockClient;
    TestServer testServer;

    CHECK_NOTHROW(testServer = makeTestServer());
    REQUIRE_NOTHROW(testServer->start());
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == CONNECTION_SUCCESSFULLY)
    {
        bindOfSendingMessage(mockClient, 
            MessageType::ServerAccept, MessageBody::ExpensiveBody);
    }
    REQUIRE_NOTHROW(testServerUpdating(testServer));
}

TEST_CASE("TestServerFailStartingUpWithBadPort [failed case]")
{   
    TestServer testServer;
    CHECK_FALSE(makeTestBadServer(testServer));
}
