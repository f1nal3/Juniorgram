#include <catch2/catch.hpp>
#include <TestUtility.hpp>

#include <thread>

using namespace TestUtility;

TEST_CASE("Check message history request of server")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::MessageHistoryRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check direct message create request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::DirectMessageCreateRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check all messages from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::MessageAll));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message store request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::MessageStoreRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message reaction request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::MessageReactionRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message edit from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::MessageEditRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message delete request from server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]() {
        Client.connectToServer(ServerInfo::Address::local, ServerInfo::Port::test);
        CHECK_NOTHROW(testSendingMessages(Client, MessageType::MessageDeleteRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
