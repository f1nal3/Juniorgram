#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <TestUtility/TestUtility.hpp>

#include <thread>

using Client      = Network::Client;
using Message     = Network::Message;
using testServer  = Server::Server;
using MessageType = Message::MessageType;

using TestUtility::testSendingMessages;
using TestUtility::testServerUpdating;

TEST_CASE("Check server ping")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::ServerPing));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check default request of server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client        Client;
    const int16_t failedType = 666;

    std::thread threadMockClient([&Client, &failedType]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType(failedType)));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
