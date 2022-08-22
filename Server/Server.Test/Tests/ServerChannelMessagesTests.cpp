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

TEST_CASE("Check channel create request of server")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::ChannelCreateRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel list request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::ChannelListRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel subscribe request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::ChannelSubscribeRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel leave request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::ChannelLeaveRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel delete request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::ChannelDeleteRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
