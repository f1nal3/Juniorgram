#pragma once

#include <ServerBuilder.hpp>

#include <MockObjectsFiles/MockDatabase.hpp>
#include <MockObjectsFiles/MockClient.hpp>
#include <MockObjectsFiles/MockRepositoryManager.hpp>
#include <MockObjectsFiles/MessageFiller.hpp>

namespace Builder
{
class ServerBuilder;
}

namespace MockDataAccess
{
class MockRepositoryManager;
}

namespace MesgFiller
{
class MessageFiller;
}

namespace TestUtility
{
using Database         = MockDatabase::MockDatabase;
using RepoManager      = MockDataAccess::MockRepositoryManager;
using RTC              = std::chrono::system_clock;
using Client           = MockClient::MockClient;
using Message          = Network::Message;
using TestServer       = std::unique_ptr<Server::Server>;
using MessageType      = Message::MessageType;
using milliseconds     = std::chrono::milliseconds;
using MessageFiller    = MesgFiller::MessageFiller;
using ServerBuilder    = Server::Builder::ServerBuilder;
using PairArguments    = std::pair<std::string, std::string>;

using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelSubscriptionInfo = Models::ChannelSubscriptionInfo;
using ChannelDeleteInfo       = Models::ChannelDeleteInfo;
using ChannelLeaveInfo        = Models::ChannelLeaveInfo;
using ChannelInfo             = Models::ChannelInfo;

constexpr bool testAcceptingConnection = true;

struct TestPairFlags
{
    TestPairFlags()  = default;
    ~TestPairFlags() = default;

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
    PairArguments  dbnamePair{"--dbname", "juniorgram"};
    PairArguments  hostPair{"--hostaddr", "127.0.0.1"};
    PairArguments  dbportPair{"--port", "5432"};
    PairArguments  userPair{"--user", "postgres"};
    PairArguments  passwordPair{"--password", "postgres"};
};

inline Message& messageInit(Message& message, MessageType messageType) noexcept
{
    message.mHeader.mMessageType = messageType;
    message.mHeader.mTimestamp   = RTC::to_time_t(RTC::now());
    
    MessageFiller mesgFiller;
    mesgFiller.fillAllMessages();

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
            message.mBody = std::make_any<std::string>(channelInfo);

            break;
        }

        case Message::MessageType::ChannelSubscriptionListRequest:
        {
            message.mBody = std::make_any<Models::ChannelSubscriptionInfo>
                (mesgFiller.getChannelInfo()._channelID);

            break;
        }

        case Message::MessageType::ChannelSubscribeRequest:
        {
            Models::ChannelSubscriptionInfo messageInfo(mesgFiller.getChannelSubscriptionInfo());       
            message.mBody          = std::any_cast<Models::ChannelSubscriptionInfo>(messageInfo);

            break;
        }

        case Message::MessageType::ChannelLeaveRequest:
        {
            std::string_view messageInfo = mesgFiller.getChannelLeaveInfo()._channelName;
            message.mBody = std::make_any<std::string>(messageInfo);

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
            message.mBody    = std::make_any<ReplyInfo>(replyInfo);

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
            message.mBody         = std::make_any<MessageInfo>(messageInfo);

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
            std::vector<Models::ChannelInfo> testChannelList;
            message.mBody = std::make_any<std::vector<Models::ChannelInfo>>(testChannelList);

            break;
        }
        default:
        {
            Base::Logger::FileLogger::getInstance().log
            (
                "Unimplemented[" + std::to_string
                (uint32_t(message.mHeader.mMessageType)) + "]",
                Base::Logger::LogLevel::WARNING
            );
            break;
        }
    }
    return message;
}

inline void testSendingMessages(const Client& Client, const MessageType mesgType) noexcept
{
    Message message;
    std::mutex scopedMutex;

    std::scoped_lock scopedLock(scopedMutex);
    messageInit(message, mesgType);
    std::this_thread::sleep_for(milliseconds(5000));
    Client.send(message);
}

constexpr TestServer& testServerUpdating(TestServer& serverTest) noexcept
{
    unsigned int countOfUpdate        = 0;
    unsigned int iterationOfServer    = 1;
    bool         isConnected          = true;

    while (isConnected)
    {
        ++countOfUpdate;
        if (countOfUpdate > iterationOfServer)
        {
            serverTest->stop();
            break;
        }
        serverTest->update();
    }

    return serverTest;
}

inline decltype(auto) bindOfSendingMessage(Client& Client, MessageType mesgType) noexcept
{
    using namespace std::placeholders;
    const auto sendingMessage = std::bind
    (
        &testSendingMessages,
        _1, _2
    );
    
   return sendingMessage(Client,mesgType);
}

inline auto bindOfConnectToServer(Client& Client, 
    const std::string_view& address, const uint16_t port) noexcept
{
    const auto connectionInit = std::bind
    (
        &Client::connectToServer, &Client, 
        address, port
    );

    return connectionInit();
}

inline auto getTestDatabase() noexcept
{
    auto testDatabase = std::make_unique<RepoManager>(Database::getInstance<Database>());
    return testDatabase;
}

inline auto makeTestServer() noexcept
{
    TestPairFlags pairFlags;
    TestServer testServer = ServerBuilder()
                                    .setValue(pairFlags.getServerPortArguments())
                                    .setValue(pairFlags.getDatabaseArguments())
                                    .setValue(pairFlags.getHostAddrArguments())
                                    .setValue(pairFlags.getDatabasePortArguments())
                                    .setValue(pairFlags.getDatabaseUserArguments())
                                    .setValue(pairFlags.getDatabasePasswordArguments())
                                    .setValue(getTestDatabase().release())
                                    .makeServer();
    return testServer;
}

inline auto makeTestBadServer() noexcept
{
    TestPairFlags pairFlags;
    TestServer    testServer = ServerBuilder()
                                .setValue(pairFlags.getBadServerPortArguments())
                                .setValue(pairFlags.getDatabaseArguments())
                                .setValue(pairFlags.getHostAddrArguments())
                                .setValue(pairFlags.getDatabasePortArguments())
                                .setValue(pairFlags.getDatabaseUserArguments())
                                .setValue(pairFlags.getDatabasePasswordArguments())
                                .setValue(getTestDatabase().release())
                                .makeServer();
    return testServer;
}

constexpr uint16_t getTestingPort() 
{
    return ServerInfo::Port::test;
}

constexpr std::string_view getTestingAddress()
{
    return ServerInfo::Address::local;
}
}  // namespace TestUtility
