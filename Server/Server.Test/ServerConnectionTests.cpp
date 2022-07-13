#include <catch2/catch.hpp>

#include <iostream>
#include <future>
#include <thread>

#include <Server.hpp>
#include <MockClient.hpp>
#include <TestUtility.hpp>

using Network::Message;
using Client     = MockClient::MockClient;
using testServer = Server::Server;
using milliseconds = std::chrono::milliseconds;

TEST_CASE("Workflow startup server")
{
    uint16_t       testPort;
    testServer serverTest(testPort);
    unsigned int            countOfUpdate = 0;
    bool                    serverWork    = true;

    CHECK_NOTHROW(serverTest.start());

    Client mockClient;

    std::thread threadServer([&]() {
        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverTest.stop();
                break;
            }
            serverTest.update();
        }

        REQUIRE_NOTHROW(serverTest.stop());
    });

    std::thread threadMockClient([&]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::ServerMessage);
        mockClient.send(message);
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.send(Message());
        std::this_thread::sleep_for(milliseconds(5000));
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

    Client mockClient;
    unsigned int  countOfUpdate = 0;
    bool             serverWork = true;

   std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (serverWork)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
   });

   std::thread threadMockClient([&mockClient, &testPort]() {
       std::this_thread::sleep_for(milliseconds(10000));
       mockClient.connectToServer(ServerInfo::Address::local, testPort);
       std::this_thread::sleep_for(milliseconds(5000));

       Message message;
       TestUtility::messageInstance(message, Message::MessageType::ServerPing);
       CHECK_NOTHROW(mockClient.send(message));
       std::this_thread::sleep_for(milliseconds(5000));
       REQUIRE_NOTHROW(mockClient.send(Message()));
       std::this_thread::sleep_for(milliseconds(5000));
       mockClient.disconnectFromServer();
   });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check registration request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);

    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest,&serverWork,&countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient,&testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::RegistrationRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check login request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient, &testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::LoginRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

//TEST_CASE("Check channel create request of server")
//{
//    uint16_t   testPort;
//    testServer serverTest(testPort);
//    int        countOfUpdate = 0;
//    bool       serverWork    = true;
//
//    Client mockClient;
//
//    std::thread threadServer([&]() {
//        serverTest.start();
//
//        while (true)
//        {
//            ++countOfUpdate;
//            if (countOfUpdate > 2)
//            {
//                serverWork = false;
//                serverTest.stop();
//                break;
//            }
//            std::this_thread::sleep_for(milliseconds(1000));
//            serverTest.update();
//        }
//    });
//
//    std::thread threadMockClient([&]() {
//        std::this_thread::sleep_for(milliseconds(10000));
//        mockClient.connectToServer(ServerInfo::Address::local, testPort);
//        std::this_thread::sleep_for(milliseconds(5000));
//
//        Message message;
//        TestUtility::messageInstance(message, Message::MessageType::ChannelCreateRequest);
//        CHECK_NOTHROW(mockClient.send(message));
//        std::this_thread::sleep_for(milliseconds(5000));
//        REQUIRE_NOTHROW(mockClient.send(Message()));
//        std::this_thread::sleep_for(milliseconds(5000));
//        mockClient.disconnectFromServer();
//    });
//
//    threadMockClient.join();
//    threadServer.join();
//}

TEST_CASE("Check message history request of server")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient, &testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::MessageHistoryRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

//TEST_CASE("Check direct message create request from server side")
//{
//    uint16_t   testPort;
//    testServer serverTest(testPort);
//    int        countOfUpdate = 0;
//    bool       serverWork    = true;
//
//    Client mockClient;
//
//    std::thread threadServer([&]() {
//        serverTest.start();
//
//        while (true)
//        {
//            ++countOfUpdate;
//            if (countOfUpdate > 2)
//            {
//                serverWork = false;
//                serverTest.stop();
//                break;
//            }
//            std::this_thread::sleep_for(milliseconds(1000));
//            serverTest.update();
//        }
//    });
//
//    std::thread threadMockClient([&]() {
//        std::this_thread::sleep_for(milliseconds(10000));
//        mockClient.connectToServer(ServerInfo::Address::local, testPort);
//        std::this_thread::sleep_for(milliseconds(5000));
//
//        Message message;
//        TestUtility::messageInstance(message, Message::MessageType::DirectMessageCreateRequest);
//        CHECK_NOTHROW(mockClient.send(message));
//        std::this_thread::sleep_for(milliseconds(5000));
//        REQUIRE_NOTHROW(mockClient.send(Message()));
//        std::this_thread::sleep_for(milliseconds(5000));
//        mockClient.disconnectFromServer();
//    });
//
//    threadMockClient.join();
//    threadServer.join();
//}

TEST_CASE("Check all messages from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient, &testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::MessageAll);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message store request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient, &testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::MessageStoreRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message reaction request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient, &testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::MessageReactionRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check message edit from server side")
{
    //uint16_t   testPort;
    //testServer serverTest(testPort);
    //int        countOfUpdate = 0;
    //bool       serverWork    = true;

    //Client mockClient;

    //std::thread threadServer([&]() {
    //    serverTest.start();

    //    while (true)
    //    {
    //        ++countOfUpdate;
    //        if (countOfUpdate > 2)
    //        {
    //            serverTest.stop();
    //            break;
    //        }
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //        serverTest.update();
    //    }
    //});

    //std::thread threadMockClient([&]() {
    //    std::this_thread::sleep_for(milliseconds(10000));
    //    mockClient.connectToServer(ServerInfo::Address::local, testPort);
    //    std::this_thread::sleep_for(milliseconds(5000));

    //    Message message;
    //     TestUtility::messageInstance(message, Message::MessageType::MessageEditRequest);
    //     CHECK_NOTHROW(mockClient.send(message));
    //    std::this_thread::sleep_for(milliseconds(5000));
    //    mockClient.send(Message());
    //    std::this_thread::sleep_for(milliseconds(5000));
    //    mockClient.disconnectFromServer();
    //});

    //threadMockClient.join();
    //threadServer.join();
}

TEST_CASE("Check message delete request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient, &testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::MessageDeleteRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

//TEST_CASE("Check channel list request from server side")
//{
//    uint16_t   testPort;
//    testServer serverTest(testPort);
//    int        countOfUpdate = 0;
//    bool       serverWork    = true;
//
//    Client mockClient;
//
//    std::thread threadServer([&]() {
//        serverTest.start();
//
//        while (true)
//        {
//            ++countOfUpdate;
//            if (countOfUpdate > 2)
//            {
//                serverTest.stop();
//                break;
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//            serverTest.update();
//        }
//    });
//
//    std::thread threadMockClient([&]() {
//        std::this_thread::sleep_for(milliseconds(10000));
//        mockClient.connectToServer(ServerInfo::Address::local, testPort);
//        std::this_thread::sleep_for(milliseconds(5000));
//
//        Message message;
//        TestUtility::messageInstance(message, Message::MessageType::ChannelListRequest);
//        CHECK_NOTHROW(mockClient.send(message));
//        std::this_thread::sleep_for(milliseconds(5000));
//        mockClient.send(Message());
//        std::this_thread::sleep_for(milliseconds(5000));
//        mockClient.disconnectFromServer();
//    });
//
//    threadMockClient.join();
//    threadServer.join();
//}

TEST_CASE("Check channel subscribe request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient,&testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::ChannelSubscribeRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel leave request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient,&testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::ChannelLeaveRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check channel delete request from server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverWork = false;
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient,&testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        Message message;
        TestUtility::messageInstance(message, Message::MessageType::ChannelDeleteRequest);
        CHECK_NOTHROW(mockClient.send(message));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        mockClient.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check default request of server side")
{
    uint16_t   testPort;
    testServer serverTest(testPort);
    unsigned int        countOfUpdate = 0;
    bool                serverWork    = true;

    Client mockClient;

    std::thread threadServer([&serverTest, &serverWork, &countOfUpdate]() {
        serverTest.start();

        while (true)
        {
            serverWork = false;
            ++countOfUpdate;
            if (countOfUpdate > 2)
            {
                serverTest.stop();
                break;
            }
            std::this_thread::sleep_for(milliseconds(1000));
            serverTest.update();
        }
    });

    std::thread threadMockClient([&mockClient,&testPort]() {
        std::this_thread::sleep_for(milliseconds(10000));
        mockClient.connectToServer(ServerInfo::Address::local, testPort);
        std::this_thread::sleep_for(milliseconds(5000));

        CHECK_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
        REQUIRE_NOTHROW(mockClient.send(Message()));
        std::this_thread::sleep_for(milliseconds(5000));
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
