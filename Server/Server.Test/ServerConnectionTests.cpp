#include <catch2/catch.hpp>

#include <iostream>
#include <future>
#include <thread>

#include <Server.hpp>
#include <MockClient.hpp>
#include <TestUtility.hpp>

using Client           = MockClient::MockClient;
using Message          = Network::Message;
using testServer       = Server::Server;
using MessageType      = Message::MessageType;
using milliseconds     = std::chrono::milliseconds;

using TestUtility::testSendingMessages;
using TestUtility::testServerUpdating;

TEST_CASE("Workflow startup server")
{
    uint16_t       testPort;
    testServer serverTest(testPort);

    CHECK_NOTHROW(serverTest.start());

    std::thread threadServer([&serverTest]() {
        testServerUpdating(serverTest);
        REQUIRE_NOTHROW(serverTest.stop());
    });

    Client mockClient;

    std::thread threadMockClient([&]() {
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ServerMessage));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

suppressWarning(4244, "-Wconversion")
suppressWarning(4242, "-Wconversion")
TEST_CASE("Workflow fail start up server")
{    
    uint32_t       testBadPort = 6666666;
    testServer     badServer(testBadPort);   

    CHECK_NOTHROW(badServer.start()); 
}
restoreWarning
restoreWarning

TEST_CASE("Check server ping")
{
    uint16_t       testPort;
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

TEST_CASE("Check registration request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

     Client mockClient;

    std::thread threadMockClient([&mockClient,&testPort]() {
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

TEST_CASE("Check channel create request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    Client mockClient;

    std::thread threadServer([&]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    std::thread threadMockClient([&]() {
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType::ChannelCreateRequest));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

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
// 
TEST_CASE("Check direct message create request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    Client mockClient;

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

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

    Client mockClient;

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

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
//
TEST_CASE("Check channel list request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    Client mockClient;

    std::thread threadServer([&serverTest]() {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    std::thread threadMockClient([&mockClient,&testPort]() {
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

TEST_CASE("Check default request of server side")
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
        CHECK_NOTHROW(testSendingMessages(mockClient, MessageType(666)));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check disconnect from client")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    REQUIRE_NOTHROW(serverTest.start());

    asio::io_context            testContext;
    Network::SafeQueue<Message> testIncomingMessagesQueue;
    CHECK_NOTHROW(serverTest.messageClient(std::shared_ptr<Network::Connection>(std::make_shared<Network::Connection>
    (
        Network::Connection::OwnerType::CLIENT, testContext, 
        asio::ip::tcp::socket(testContext), testIncomingMessagesQueue)), Message())
    );
}
