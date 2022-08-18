#include <catch2/catch.hpp>
#include <TestUtility/TestUtility.hpp>

#include <thread>

using Client      = MockClient::MockClient;
using Message     = Network::Message;
using testServer  = Server::Server;
using MessageType = Message::MessageType;

using TestUtility::testSendingMessages;
using TestUtility::testServerUpdating;

TEST_CASE("Check server ping")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ServerPing));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check default request of server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client        mockClient;
    const int16_t failedType = 666;

    std::thread threadMockClient([&mockClient, &testPort, &failedType]() {
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType(failedType)));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
