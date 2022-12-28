#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Workflow startup server")
{
    Client     mockClient;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    CHECK_NOTHROW(serverTest.start());
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        bindOfSendingMessage(mockClient, MessageType::ServerAccept);
    }
    REQUIRE_NOTHROW(testServerUpdating(serverTest));
}

suppressWarning(4244, "-Wconversion")
suppressWarning(4242, "-Wconversion")
TEST_CASE("Workflow fail startup server")
{    
    uint32_t       testBadPort = 6666666;
    testServer badServer(testBadPort, getTestingDatabase());   

    CHECK_NOTHROW(badServer.start()); 
}
restoreWarning
restoreWarning

TEST_CASE("Check disconnect from client")
{
    testServer serverTest(getTestingPort());

    REQUIRE_NOTHROW(serverTest.start());

    asio::io_context            testContext;
    Utility::SafeQueue<Message> testIncomingMessagesQueue;

    CHECK_NOTHROW(serverTest.messageClient
    (
    std::shared_ptr<Network::Connection>(std::make_shared<Network::Connection>
    (
        Network::Connection::OwnerType::CLIENT, testContext,
        asio::ip::tcp::socket(testContext), testIncomingMessagesQueue)), Message()
    ));
}
