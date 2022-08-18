#include <catch2/catch.hpp>
#include <TestUtility/TestUtility.hpp>

#include <thread>

using Client      = MockClient::MockClient;
using Message     = Network::Message;
using testServer  = Server::Server;
using MessageType = Message::MessageType;

using TestUtility::testSendingMessages;
using TestUtility::testServerUpdating;

TEST_CASE("Check registration request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client mockClient;

    std::thread threadMockClient([&mockClient, &testPort]() {
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::RegistrationRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check login request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client mockClient;

    std::thread threadMockClient([&mockClient, &testPort]() {
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::LoginRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
