#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <Client.Core/Client.Core.Static/Client.hpp>
#include <Client.Core/Client.Core.Static/ServerInfo.hpp>
#include <Server.hpp>

#include <thread>

using Client           = Network::Client;
using Message          = Network::Message;
using testServer       = Server::Server;
using MessageType      = Message::MessageType;

TEST_CASE("Workflow startup server")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        CHECK_NOTHROW(serverTest.start());
        REQUIRE_NOTHROW(serverTest.stop());
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        CHECK_NOTHROW(Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test));
        REQUIRE_NOTHROW(Client.disconnectFromServer());
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
    testServer serverTest(ServerInfo::Port::test);

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
