#include <catch2/catch.hpp>
#include <TestUtility/TestUtility.hpp>

#include <thread>

using Client      = MockClient::MockClient;
using Message     = Network::Message;
using testServer  = Server::Server;
using MessageType = Message::MessageType;

using TestUtility::testSendingMessages;
using TestUtility::testServerUpdating;

TEST_CASE("Check channel create request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    std::thread threadServer([&]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client mockClient;

    std::thread threadMockClient([&mockClient, &testPort]() {
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ChannelCreateRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel list request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ChannelListRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel subscribe request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ChannelSubscribeRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel leave request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ChannelLeaveRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel delete request from server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ChannelDeleteRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
