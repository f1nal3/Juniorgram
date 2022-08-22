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

TEST_CASE("Check registration request of server")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::RegistrationRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check login request of server")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::LoginRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}