#include <catch2/catch.hpp>
#include <TestUtility/TestUtility.hpp>
#include <thread>

using Client      = MockClient::MockClient;
using Message     = Network::Message;
using testServer  = Server::Server;
using MessageType = Message::MessageType;

using TestUtility::testSendingMessages;
using TestUtility::testServerUpdating;

TEST_CASE("Check message history request of server")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::MessageHistoryRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check direct message create request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::DirectMessageCreateRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check all messages from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::MessageAll));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message store request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::MessageStoreRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message reaction request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::MessageReactionRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message edit from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::MessageEditRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message delete request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::MessageDeleteRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
