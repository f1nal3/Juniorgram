#include <catch2/catch.hpp>
#include <MockObjects/MockClient.hpp>
#include <Server.hpp>

#include <thread>

using Client           = MockClient::MockClient;
using Message          = Network::Message;
using testServer       = Server::Server;
using MessageType      = Message::MessageType;

TEST_CASE("Workflow startup server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    std::thread threadServer([&serverTest]() {
        CHECK_NOTHROW(serverTest.start());
        REQUIRE_NOTHROW(serverTest.stop());
    });

    Client mockClient;

    std::thread threadMockClient([&mockClient, &testPort]() {
        CHECK_NOTHROW(mockClient.connectToServer(ServerInfo::Address::local, testPort));
        REQUIRE_NOTHROW(mockClient.disconnectFromServer());
    });

    threadMockClient.join();
    threadServer.join();
}

suppressWarning(4244, "-Wconversion")
suppressWarning(4242, "-Wconversion")
TEST_CASE("Workflow fail startup server")
{    
    uint32_t       testBadPort = 6666666;
    testServer     badServer(testBadPort);   

    CHECK_NOTHROW(badServer.start()); 
}
restoreWarning
restoreWarning

TEST_CASE("Check disconnect from client")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

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
