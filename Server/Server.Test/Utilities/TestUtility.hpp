#pragma once

#include <ServerBuilder.hpp>

#include <DataAccess.Mock/MockDatabase.hpp>
#include <DataAccess.Mock/MockRepositoryManager.hpp>
#include <Client.Test/TestClient.hpp>

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
using TestClient       = TestObject::TestClient;
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

/**
* @brief enum class MessageBody
* @details This enum class is designed to index the initialization of the message body: / 
*          Invalid - bad body initialized with empty to test failed tests. /
*          Valid   - body with testable data for successful tests.
*/
enum class MessageBody
{
    ValidBody,
    InvalidBody
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
* @param Message& message(message body), MessageType messageType(type of message), 
*        MessageBody messageBody(body of message). /
*/
inline Message& makeMessage(Message& message, MessageType messageType, MessageBody messageBody) noexcept
{
    message.mHeader.mMessageType = messageType;
    message.mHeader.mTimestamp   = RTC::to_time_t(RTC::now());
    
    MessageFiller mesgFiller;
    mesgFiller.fillAllMessages();

    switch (messageType)
    {
        case Message::MessageType::RegistrationRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                RegistrationInfo registrationInfo;
                registrationInfo._login = "";
                message.mBody           = std::make_any<RegistrationInfo>(registrationInfo);
                break;
            }

            RegistrationInfo registrationInfo(mesgFiller.getRegistrationInfo());
            message.mBody = std::make_any<RegistrationInfo>(registrationInfo);

            break;
        }
        case Message::MessageType::LoginRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                LoginInfo loginInfo;
                loginInfo._login = "";
                message.mBody    = std::make_any<LoginInfo>(loginInfo);
                break;
            }

            LoginInfo loginInfo(mesgFiller.getLoginInfo());
            message.mBody = std::make_any<LoginInfo>(loginInfo);

            break;
        }
        case Message::MessageType::ChannelCreateRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                std::string_view channelInfo = "";
                message.mBody                = std::make_any<std::string>(channelInfo);
                break;
            }

            std::string_view channelInfo = mesgFiller.getChannelInfo()._channelName;
            message.mBody                = std::make_any<std::string>(channelInfo);

            break;
        }
        case Message::MessageType::ChannelSubscriptionListRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                Models::ChannelSubscriptionInfo channelInfo;
                channelInfo._channelID = 0;
                channelInfo._userID    = 0;
                message.mBody          = std::make_any<ChannelSubscriptionInfo>(channelInfo);
                break;
            }

            message.mBody = std::make_any<ChannelSubscriptionInfo>(mesgFiller.getChannelInfo()._channelID);

            break;
        }
        case Message::MessageType::ChannelSubscribeRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                Models::ChannelSubscriptionInfo messageInfo;
                messageInfo._channelID = 0;
                messageInfo._userID    = 0;
                message.mBody          = std::any_cast<ChannelSubscriptionInfo>(messageInfo);
                break;
            }

            Models::ChannelSubscriptionInfo messageInfo(mesgFiller.getChannelSubscriptionInfo());
            message.mBody = std::any_cast<ChannelSubscriptionInfo>(messageInfo);

            break;
        }
        case Message::MessageType::ChannelLeaveRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                std::string_view messageInfo = "";
                message.mBody                = std::make_any<std::string>(messageInfo);
                break;
            }

            std::string_view messageInfo = mesgFiller.getChannelLeaveInfo()._channelName;
            message.mBody                = std::make_any<std::string>(messageInfo);

            break;
        }
        case Message::MessageType::ReplyHistoryRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                message.mBody = std::make_any<uint64_t>(0);
                break;
            }

            message.mBody = std::make_any<uint64_t>(mesgFiller.getChannelInfo()._channelID);

            break;
        }
        case Message::MessageType::ReplyStoreRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                ReplyInfo replyInfo;
                replyInfo._channelID = 0;
                message.mBody        = std::make_any<ReplyInfo>(replyInfo);
                break;
            }

            ReplyInfo replyInfo(mesgFiller.getReplyInfo());
            message.mBody = std::make_any<ReplyInfo>(replyInfo);

            break;
        }
        case Message::MessageType::MessageReactionRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                MessageInfo messageInfo;
                messageInfo._msgID = 0;
                message.mBody      = std::make_any<MessageInfo>(messageInfo);
                break;
            }

            MessageInfo messageInfo(mesgFiller.getMessageInfo());
            message.mBody = std::make_any<MessageInfo>(messageInfo);

            break;
        }
        case Message::MessageType::DirectMessageCreateRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                message.mBody = std::make_any<uint64_t>(0);
                break;
            }

            message.mBody = std::make_any<uint64_t>(mesgFiller.getMessageInfo()._recipientID);

            break;
        }
        case Message::MessageType::MessageHistoryRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                message.mBody = std::make_any<uint64_t>(0);
                break;
            }

            message.mBody = std::make_any<uint64_t>(mesgFiller.getChannelInfo()._channelID);

            break;
        }
        case Message::MessageType::MessageStoreRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                MessageInfo messageInfo;
                messageInfo._channelID = 0;
                message.mBody          = std::make_any<MessageInfo>(messageInfo);
                break;
            }

            MessageInfo messageInfo(mesgFiller.getMessageInfo());
            message.mBody = std::make_any<MessageInfo>(messageInfo);

            break;
        }
        case Message::MessageType::MessageEditRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                MessageInfo messageInfo;
                messageInfo._msgID = 0;
                message.mBody      = std::make_any<MessageInfo>(messageInfo);
                break;
            }

            MessageInfo messageInfo(mesgFiller.getMessageInfo());
            message.mBody = std::make_any<MessageInfo>(messageInfo);

            break;
        }
        case Message::MessageType::MessageDeleteRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                MessageInfo messageInfo;
                messageInfo._msgID = 0;
                message.mBody      = std::make_any<MessageInfo>(messageInfo);
                break;
            }

            MessageInfo messageInfo(mesgFiller.getMessageInfo());
            message.mBody = std::make_any<MessageInfo>(messageInfo);

            break;
        }
        case Message::MessageType::ChannelDeleteRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                Models::ChannelDeleteInfo channelDeleteInfo;
                channelDeleteInfo._channelID = 0;
                message.mBody                = std::make_any<std::string>(channelDeleteInfo._channelName);
                break;
            }

            Models::ChannelDeleteInfo channelDeleteInfo(mesgFiller.getChannelDeleteInfo());
            message.mBody = std::make_any<std::string>(channelDeleteInfo._channelName);

            break;
        }
        case Message::MessageType::ChannelListRequest:
        {
            if (messageBody == MessageBody::InvalidBody)
            {
                message.mBody = std::make_any<std::vector<ChannelInfo>>(0);
                break;
            }

            message.mBody = std::make_any<std::vector<ChannelInfo>>(mesgFiller.getChannelInfo()._channelID);

            break;
        }
        default:
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "[TestUtilityMessage]: Unimplemented["
                + std::to_string(uint32_t(message.mHeader.mMessageType)) + "]",
                Base::Logger::LogLevel::WARNING
            );
            break;
        }
    }
    return message;
}

/**
* @brief Method for getting a test database.
* @details This database is a mock object that is used for testing. 
*          The database is registered through the MockRepositoryManager(TestRepoManager), / 
*                          which internally initializes repositories. 
*/
inline auto getTestDatabase() noexcept
{
    auto testDatabase = std::make_unique<TestRepoManager>
        (TestDatabase::getInstance<TestDatabase>());

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
* @brief Method for configuring the bad test server.
* @details The server is configured through the ServerBuilder. /
*          The arguments that we set for testing are in the ConfigArguments structure. /
*          If you need to set other arguments, you can do so in the ConfigArguments structure. /
*          At the moment the server does not throw an exception if the port is invalid. 
*/
inline auto makeBadTestServer()
{
    ConfigArguments configArgs;
    TestServer testServer = ServerBuilder()
                        .setValue(configArgs.getBadServerPortArguments())
                        .setValue(configArgs.getDatabaseArguments())
                        .setValue(configArgs.getHostAddrArguments())
                        .setValue(configArgs.getDatabasePortArguments())
                        .setValue(configArgs.getDatabaseUserArguments())
                        .setValue(configArgs.getDatabasePasswordArguments())
                        .setValue(getTestDatabase().release())
                        .makeServer();
    return testServer;
}
}  /// namespace TestUtility
