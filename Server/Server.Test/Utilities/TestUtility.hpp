#pragma once

#include <ServerBuilder.hpp>

#include <DataAccess.Mock/MockDatabase.hpp>
#include <DataAccess.Mock/MockRepositoryManager.hpp>
#include <Client.Mock/MockClient.hpp>

#include "MessageFiller.hpp"

namespace Builder
{
class ServerBuilder;
}

namespace MockDataAccess
{
class MockRepositoryManager;
}

/**
* @brief namespace Test Utility.
* @details This namespace is for testing the server.
*/
namespace TestUtility
{
using RTC              = std::chrono::system_clock;
using TestClient       = MockObject::MockClient;
using Message          = Network::Message;
using TestDatabase     = MockObject::MockDatabase;
using TestServer       = std::unique_ptr<Server::Server>;
using TestRepoManager  = MockObject::MockRepositoryManager;
using MessageType      = Message::MessageType;
using milliseconds     = std::chrono::milliseconds;
using ServerBuilder    = Server::Builder::ServerBuilder;
using PairArguments    = std::pair<std::string, std::string>;

/**
* @brief These names are for messages.
*/
using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelSubscriptionInfo = Models::ChannelSubscriptionInfo;
using ChannelDeleteInfo       = Models::ChannelDeleteInfo;
using ChannelLeaveInfo        = Models::ChannelLeaveInfo;
using ChannelInfo             = Models::ChannelInfo;

constexpr bool CONNECTION_SUCCESSFULLY = true;

/**
* @brief enum class MessageBody
* @details This enum class is designed to index the initialization of the message body: / 
*          Poor - bad body initialized with empty to test failed tests.
*          Expensive - body with testable data for successful tests.
*/
enum class MessageBody
{
    PoorBody,
    ExpensiveBody
};

/**
* @brief struct ConfigArguments.
* @details Designed to receive arguments from the ServerBuilder.
*          This structure has only get methods. If you want to change the values of the arguments, /
*                                                  you can do it manually in the private section.
*/
struct ConfigArguments
{
    ConfigArguments() = default;
    ~ConfigArguments() = default;

    PairArguments& getServerPortArguments() { return serverPortPair; }
    PairArguments& getBadServerPortArguments() { return badServerPortPair; }
    PairArguments& getDatabaseArguments() { return dbnamePair; }
    PairArguments& getHostAddrArguments() { return hostPair; }
    PairArguments& getDatabasePortArguments() { return dbportPair; }
    PairArguments& getDatabaseUserArguments() { return userPair; }
    PairArguments& getDatabasePasswordArguments() { return passwordPair; }

private:
    PairArguments  serverPortPair{"--serverport", "65001"};
    PairArguments  badServerPortPair{"--serverport", "666666"};
    PairArguments  dbnamePair{"--dbname", "mockdb"};
    PairArguments  hostPair{"--hostaddr", "127.0.0.1"};
    PairArguments  dbportPair{"--port", "5432"};
    PairArguments  userPair{"--user", "tester"};
    PairArguments  passwordPair{"--password", "tester"};
};

/**
* @brief Method for making message.
* @details This method takes the body of an empty message as well as its type. /
*                      After receiving the type, the message is filled with data.
* @param Message& message(message body), MessageType messageType(type of message).
*/
inline Message& makeMessage(Message& message, MessageType messageType) noexcept
{
    message.mHeader.mMessageType = messageType;
    message.mHeader.mTimestamp   = RTC::to_time_t(RTC::now());
    
    MessageFiller mesgFiller;
    mesgFiller.fillAllMessages();

    try
    {
        switch (messageType)
        {
            case Message::MessageType::RegistrationRequest:
            {
                RegistrationInfo registrationInfo(mesgFiller.getRegistrationInfo());
                message.mBody = std::make_any<RegistrationInfo>(registrationInfo);

                break;
            }

            case Message::MessageType::LoginRequest:
            {
                LoginInfo loginInfo(mesgFiller.getLoginInfo());
                message.mBody = std::make_any<LoginInfo>(loginInfo);

                break;
            }

            case Message::MessageType::ChannelCreateRequest:
            {
                std::string_view channelInfo = mesgFiller.getChannelInfo()._channelName;
                message.mBody                = std::make_any<std::string>(channelInfo);

                break;
            }

            case Message::MessageType::ChannelSubscriptionListRequest:
            {
                message.mBody = std::make_any<ChannelSubscriptionInfo>
                    (mesgFiller.getChannelInfo()._channelID);

                break;
            }

            case Message::MessageType::ChannelSubscribeRequest:
            {
                Models::ChannelSubscriptionInfo messageInfo(mesgFiller.getChannelSubscriptionInfo());
                message.mBody = std::any_cast<ChannelSubscriptionInfo>(messageInfo);

                break;
            }

            case Message::MessageType::ChannelLeaveRequest:
            {
                std::string_view messageInfo = mesgFiller.getChannelLeaveInfo()._channelName;
                message.mBody                = std::make_any<std::string>(messageInfo);

                break;
            }

            case Message::MessageType::ReplyHistoryRequest:
            {
                message.mBody = std::make_any<uint64_t>(mesgFiller.getChannelInfo()._channelID);

                break;
            }

            case Message::MessageType::ReplyStoreRequest:
            {
                ReplyInfo replyInfo(mesgFiller.getReplyInfo());
                message.mBody = std::make_any<ReplyInfo>(replyInfo);

                break;
            }

            case Message::MessageType::MessageReactionRequest:
            {
                MessageInfo messageInfo(mesgFiller.getMessageInfo());
                message.mBody = std::make_any<MessageInfo>(messageInfo);

                break;
            }

            case Message::MessageType::DirectMessageCreateRequest:
            {
                message.mBody = std::make_any<uint64_t>(mesgFiller.getMessageInfo()._recipientID);

                break;
            }

            case Message::MessageType::MessageHistoryRequest:
            {
                message.mBody = std::make_any<uint64_t>(mesgFiller.getChannelInfo()._channelID);

                break;
            }

            case Message::MessageType::MessageStoreRequest:
            {
                MessageInfo messageInfo(mesgFiller.getMessageInfo());
                message.mBody = std::make_any<MessageInfo>(messageInfo);

                break;
            }

            case Message::MessageType::MessageEditRequest:
            {
                MessageInfo messageInfo(mesgFiller.getMessageInfo());
                message.mBody = std::make_any<MessageInfo>(messageInfo);

                break;
            }

            case Message::MessageType::MessageDeleteRequest:
            {
                MessageInfo messageInfo(mesgFiller.getMessageInfo());
                message.mBody = std::make_any<MessageInfo>(messageInfo);

                break;
            }

            case Message::MessageType::ChannelDeleteRequest:
            {
                Models::ChannelDeleteInfo channelDeleteInfo(mesgFiller.getChannelDeleteInfo());
                message.mBody = std::make_any<std::string>(channelDeleteInfo._channelName);

                break;
            }

            case Message::MessageType::ChannelListRequest:
            {
                message.mBody = std::make_any<std::vector<ChannelInfo>>
                    (mesgFiller.getChannelInfo()._channelID);

                break;
            }
            case Message::MessageType::ServerAccept:
            {
                Base::Logger::FileLogger::getInstance().log
                (
                    "[TestUtilityMessage]: Server accepted the connection!",
                     Base::Logger::LogLevel::INFO
                );

                break;
            }
            default:
            {
                Base::Logger::FileLogger::getInstance().log
                (
                    "[TestUtilityMessage]: Unimplemented[" + std::to_string
                    (uint32_t(message.mHeader.mMessageType)) + "]",
                     Base::Logger::LogLevel::WARNING
                );
                throw std::invalid_argument("Unimplemented!");

                break;
            }
        }
    }
    catch (const std::invalid_argument& testException)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            testException.what(),
            Base::Logger::LogLevel::WARNING
        );
    }
    return message;
}

/**
* @brief Method for making bad message.
* @details This method takes the body of an empty message as well as its type. /
*                      After receiving the type, the message is filled with bad data.
* @param Message& message(message body), MessageType messageType(type of message).
*/
inline Message& makePoorMessage(Message& message, MessageType messageType) noexcept
{
    message.mHeader.mMessageType = messageType;
    message.mHeader.mTimestamp   = RTC::to_time_t(RTC::now());

    switch (messageType)
    {
        case Message::MessageType::RegistrationRequest:
        {
            RegistrationInfo registrationInfo;
            registrationInfo._login = "";
            message.mBody           = std::make_any<RegistrationInfo>(registrationInfo);

            break;
        }

        case Message::MessageType::LoginRequest:
        {
            LoginInfo loginInfo;
            loginInfo._login = "";
            message.mBody    = std::make_any<LoginInfo>(loginInfo);

            break;
        }

        case Message::MessageType::ChannelCreateRequest:
        {
            std::string_view channelInfo = "";
            message.mBody                = std::make_any<std::string>(channelInfo);

            break;
        }

        case Message::MessageType::ChannelSubscriptionListRequest:
        {
            Models::ChannelSubscriptionInfo channelInfo;
            channelInfo._channelID = 0;
            channelInfo._userID    = 0;
            message.mBody          = std::make_any<ChannelSubscriptionInfo>(channelInfo);

            break;
        }

        case Message::MessageType::ChannelSubscribeRequest:
        {
            Models::ChannelSubscriptionInfo messageInfo;
            messageInfo._channelID = 0;
            messageInfo._userID    = 0;
            message.mBody          = std::any_cast<ChannelSubscriptionInfo>(messageInfo);

            break;
        }

        case Message::MessageType::ChannelLeaveRequest:
        {
            std::string_view messageInfo = "";
            message.mBody                = std::make_any<std::string>(messageInfo);

            break;
        }

        case Message::MessageType::ReplyHistoryRequest:
        {
            message.mBody = std::make_any<uint64_t>(0);

            break;
        }

        case Message::MessageType::ReplyStoreRequest:
        {
            ReplyInfo replyInfo;
            replyInfo._channelID = 0;
            message.mBody        = std::make_any<ReplyInfo>(replyInfo);

            break;
        }

        case Message::MessageType::MessageReactionRequest:
        {
            MessageInfo messageInfo;
            messageInfo._msgID = 0;
            message.mBody      = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::DirectMessageCreateRequest:
        {
            message.mBody = std::make_any<uint64_t>(0);

            break;
        }

        case Message::MessageType::MessageHistoryRequest:
        {
            message.mBody = std::make_any<uint64_t>(0);

            break;
        }

        case Message::MessageType::MessageStoreRequest:
        {
            MessageInfo messageInfo;
            messageInfo._channelID = 0;
            message.mBody          = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::MessageEditRequest:
        {
            MessageInfo messageInfo;
            messageInfo._msgID = 0;
            message.mBody      = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::MessageDeleteRequest:
        {
            MessageInfo messageInfo;
            messageInfo._msgID = 0;
            message.mBody      = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::ChannelDeleteRequest:
        {
            Models::ChannelDeleteInfo channelDeleteInfo;
            channelDeleteInfo._channelID = 0;
            message.mBody                = std::make_any<std::string>(channelDeleteInfo._channelName);

            break;
        }

        case Message::MessageType::ChannelListRequest:
        {
            message.mBody = std::make_any<std::vector<ChannelInfo>>(0);

            break;
        }

        case Message::MessageType::ServerAccept:
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "[TestUtilityMessage]: Server didn't accepted the connection!",
                 Base::Logger::LogLevel::INFO
            );

            break;
        }

        default:
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "[TestUtilityMessage]: Bad implemented[" + 
                std::to_string(uint32_t(message.mHeader.mMessageType)) + "]",
                Base::Logger::LogLevel::WARNING
            );

            break;
        }
    }
    return message;
}

/**
* @brief Method for sending messages to client part.
* @details Another method (makeMessage) is called inside the method to generate the message. /
*          After the message is generated, it is sent to the client side for further processing. /
* @param const TestClient& Client(Client object), const MessageType mesgType(type of message), /
*        const MessageBody mesgBody(Body properties for testing).
*/
inline void testSendingMessages(const TestClient& Client, const MessageType mesgType, const MessageBody mesgBody) noexcept
{
    Message    message;
    std::mutex scopedMutex;

    std::scoped_lock scopedLock(scopedMutex);
    if (mesgBody == MessageBody::PoorBody)
    {
        makePoorMessage(message, mesgType);
        std::this_thread::sleep_for(milliseconds(1000));
        Client.send(message);
    }
    else
    {
        makeMessage(message, mesgType);
        std::this_thread::sleep_for(milliseconds(1000));
        Client.send(message);
    }
}

/**
* @brief Method for updating server.
* @details The logic of this method counts on one server update to handle one message from the client side. /
*          After acceptance, the server gives its response and stops it's work.
* @param TestServer& server(test server).
*/
constexpr TestServer& testServerUpdating(TestServer& server) noexcept
{
    unsigned int countOfUpdate        = 0;
    unsigned int iterationOfServer    = 1;
    bool         isConnected          = true;

    while (isConnected)
    {
        std::this_thread::sleep_for(milliseconds(1000));
        ++countOfUpdate;
        if (countOfUpdate > iterationOfServer)
        {
            server->stop();
            break;
        }
        server->update();
    }

    return server;
}

/**
* @brief Message binding method.
* @details This method binds method arguments to initialize the testSendingMessages method. /
*          It simplifies the code and makes it compact. 
* @param TestClient& Client(Client object), MessageType mesgType(type of message), /
*        const MessageBody mesgBody(Body properties for testing).
*/
inline decltype(auto) bindOfSendingMessage(TestClient& Client, MessageType mesgType, const MessageBody mesgBody) noexcept
{   
    using namespace std::placeholders;
    if (mesgBody == MessageBody::PoorBody)
    {
        const auto sendingBadMessage = std::bind
        (
            &testSendingMessages, _1,_2,_3
        );
        return sendingBadMessage(Client, mesgType, mesgBody);
    }

    const auto sendingMessage = std::bind
    (
        &testSendingMessages, _1, _2,_3
    );

    return sendingMessage(Client, mesgType, mesgBody);
}

/**
* @brief Connect to server binding method.
* @details This method binds method arguments to initialize the Client::connectToServer method./
*          It simplifies the code and makes it compact.
* @param TestClient& Client(Client object), const std::string_view& address(ip adress for connection), /
*        const uint16_t port(port for connection).
*/
inline auto bindOfConnectToServer(TestClient& Client, 
    const std::string_view& address, const uint16_t port) noexcept
{
    const auto connectionInit = std::bind
    (
        &TestClient::connectToServer, &Client, 
        address, port
    );

    return connectionInit();
}

/**
* @brief Method for getting a test database.
* @details This database is a mock object that is used for testing. 
*          The database is registered through the MockRepositoryManager(TestRepoManager), / 
*                          which internally initializes repositories. 
*/
inline auto getTestDatabase() noexcept
{
    auto testDatabase = std::make_unique<TestRepoManager>(TestDatabase::getInstance<TestDatabase>());
    return testDatabase;
}

/**
* @brief Method for configuring the test server.
* @details The server is configured through the ServerBuilder. 
*          The arguments that we set for testing are in the ConfigArguments structure.
*          If you need to set other arguments, you can do so in the ConfigArguments structure.
*/
inline auto makeTestServer() noexcept
{
    ConfigArguments configArgs;
    TestServer testServer = ServerBuilder()
                                .setValue(configArgs.getServerPortArguments())
                                .setValue(configArgs.getDatabaseArguments())
                                .setValue(configArgs.getHostAddrArguments())
                                .setValue(configArgs.getDatabasePortArguments())
                                .setValue(configArgs.getDatabaseUserArguments())
                                .setValue(configArgs.getDatabasePasswordArguments())
                                .setValue(getTestDatabase().release())
                                .makeServer();
    return testServer;
}

/**
* @brief Method for configuring the test bad server.
* @details The server is configured through the ServerBuilder. /
*          The arguments that we set for testing are in the ConfigArguments structure. /
*          If you need to set other arguments, you can do so in the ConfigArguments structure. /
*          At the moment the server does not throw an exception if the port is invalid. 
*/
suppressWarning(4457, "-Wconversion")
suppressWarning(4702, "-Wconversion")
suppressWarning(4100, "-Wunused-parameter")
inline auto makeTestBadServer(TestServer& testServer)
{
    try
    {
        throw std::invalid_argument("Bad Server port!");

        ConfigArguments configArgs;
        testServer = ServerBuilder()
                                    .setValue(configArgs.getBadServerPortArguments())
                                    .setValue(configArgs.getDatabaseArguments())
                                    .setValue(configArgs.getHostAddrArguments())
                                    .setValue(configArgs.getDatabasePortArguments())
                                    .setValue(configArgs.getDatabaseUserArguments())
                                    .setValue(configArgs.getDatabasePasswordArguments())
                                    .setValue(getTestDatabase().release())
                                    .makeServer();
        return testServer.get();
    }
    catch (const std::invalid_argument& badServerPort)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            badServerPort.what(),
            Base::Logger::LogLevel::WARNING
        );
        return TestServer().release();
    }
}
restoreWarning
restoreWarning
restoreWarning

/**
* @brief Method for getting a test port.
* @details The return value comes from the namespace with the given values.
*/
constexpr uint16_t getTestingPort() 
{
    return ServerInfo::Port::test;
}

/**
* @brief Method for getting a test address.
* @details The return value comes from the namespace with the given values.
*/
constexpr std::string_view getTestingAddress()
{
    return ServerInfo::Address::local;
}
}  /// namespace TestUtility
