#pragma once

#include "Server.hpp"
#include "../MockObjects/MockClient.hpp"
#include "../MockObjects/MockDatabase.hpp"
#include "Cryptography.hpp"

#include <DataAccess.Postgre/PostgreRepositoryManager.hpp>

namespace TestUtility
{
using RTC              = std::chrono::system_clock;
using Client           = MockClient::MockClient;
using Message          = Network::Message;
using testServer       = Server::Server;
using MessageType      = Message::MessageType;
using milliseconds     = std::chrono::milliseconds;

struct UserInfo
{
    UserInfo() = default;
    ~UserInfo() = default;

    const std::string& getUserLogin() const { return testLogin; }
    const std::string& getUserEmail() const { return testEmail; }
    const std::string& getUserPassword() const { return testPSWD; }

private:
    const std::string testEmail = "demonstakingoverme@epam.co";
    const std::string testLogin{"memorisecodead"};
    const std::string testPSWD{"12juniorgramMargroinuj"};
};  

inline Message& messageInit(Message& message, MessageType messageType) noexcept
{
using LoginInfo        = Models::LoginInfo;
using ReplyInfo        = Models::ReplyInfo;
using MessageInfo      = Models::MessageInfo;
using RegistrationInfo = Models::RegistrationInfo;

    UserInfo userInfo;
    const std::string testPWDHash = Base::Hashing::SHA_256
    (
        userInfo.getUserPassword(), userInfo.getUserLogin()
    );

    const std::string testChannelName = "testServer";
    const std::string testMessage     = "Hello, juniorgram!!";

    constexpr uint16_t           testReactionID  = 0;
    constexpr uint64_t           testMsgID       = 2;
    constexpr uint64_t           testReceiverID  = 1;
    constexpr uint64_t           testUserID      = 1;
    constexpr uint64_t           testChannelID   = 1;

    message.mHeader.mMessageType = messageType;
    message.mHeader.mTimestamp   = RTC::to_time_t(RTC::now());

    switch (messageType)
    {
        case Message::MessageType::RegistrationRequest:
        {
            RegistrationInfo registrationInfo(userInfo.getUserEmail(), userInfo.getUserLogin(), testPWDHash);
            message.mBody = std::make_any<RegistrationInfo>(registrationInfo);

            break;
        }

        case Message::MessageType::LoginRequest:
        {
            LoginInfo loginInfo(userInfo.getUserLogin(), testPWDHash);
            message.mBody = std::make_any<LoginInfo>(loginInfo);

            break;
        }

        case Message::MessageType::ChannelCreateRequest:
        {
            std::string_view channelInfo = testChannelName;
            message.mBody           = std::make_any<std::string>(channelInfo);

            break;
        }

        case Message::MessageType::ChannelSubscriptionListRequest:
        {
            message.mBody = std::make_any<Models::ChannelSubscriptionInfo>(testChannelID);

            break;
        }

        case Message::MessageType::ChannelSubscribeRequest:
        {
            Models::ChannelSubscriptionInfo messageInfo(testChannelID);       
            messageInfo._userID    = testUserID;
            message.mBody          = std::any_cast<Models::ChannelSubscriptionInfo>(messageInfo);

            break;
        }

        case Message::MessageType::ChannelLeaveRequest:
        {
            std::string_view messageInfo = testChannelName;
            message.mBody           = std::make_any<std::string>(messageInfo);

            break;
        }

        case Message::MessageType::ReplyHistoryRequest:
        {
            message.mBody = std::make_any<uint64_t>(testChannelID);

            break;
        }

        case Message::MessageType::ReplyStoreRequest:
        {
            ReplyInfo replyInfo(testChannelID,testMessage);
            replyInfo._msgID     = testMsgID;
            message.mBody        = std::make_any<ReplyInfo>(replyInfo);

            break;
        }

        case Message::MessageType::MessageReactionRequest:
        {
            MessageInfo messageInfo;
            messageInfo._msgID                     = testMsgID;
            messageInfo._reactions[testReactionID] = std::numeric_limits<uint32_t>::max();
            message.mBody                          = std::make_any<MessageInfo>(messageInfo);

            break;
        }
        
        case Message::MessageType::DirectMessageCreateRequest:
        {
            message.mBody = std::make_any<uint64_t>(testReceiverID);

            break;
        }

        case Message::MessageType::MessageHistoryRequest:
        {
            message.mBody = std::make_any<uint64_t>(testChannelID);

            break;
        }

        case Message::MessageType::MessageStoreRequest:
        {
            MessageInfo messageInfo(testChannelID,testMessage);
            message.mBody = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::MessageEditRequest:
        {
            MessageInfo messageInfo(testChannelID, testMessage);
            messageInfo._senderID = testUserID;
            messageInfo._msgID    = testMsgID;
            message.mBody           = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::MessageDeleteRequest:
        {
            MessageInfo messageInfo(testChannelID, testMessage);
            message.mBody = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::ChannelDeleteRequest:
        {
            Models::ChannelDeleteInfo channelDeleteInfo;
            channelDeleteInfo._creatorID   = testUserID;
            channelDeleteInfo._channelName = testChannelName;
            message.mBody                  = std::make_any<std::string>(testChannelName);

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

inline void [[nodiscard]] testSendingMessages(const Client& Client, const MessageType mesgType) noexcept
{
    Message message;
    std::mutex scopedMutex;

    std::scoped_lock scopedLock(scopedMutex);
    messageInit(message, mesgType);
    std::this_thread::sleep_for(milliseconds(5000));
    Client.send(message);
}

constexpr testServer& testServerUpdating(testServer& serverTest) noexcept
{
    unsigned int countOfUpdate        = 0;
    unsigned int iterationOfServer    = 1;
    bool         isConnected          = true;

    while (isConnected)
    {
        ++countOfUpdate;
        if (countOfUpdate > iterationOfServer)
        {
            serverTest.stop();
            break;
        }
        serverTest.update();
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

constexpr uint16_t getTestingPort() 
{
    return ServerInfo::Port::test;
}

constexpr std::string_view getTestingAddress()
{
    return ServerInfo::Address::local;
}

inline auto getTestingDatabase() noexcept
{
    std::unique_ptr<DataAccess::PostgreRepositoryManager> testDatabase =
        std::make_unique<DataAccess::PostgreRepositoryManager>(MockDatabase::MockDatabase::getInstance<MockDatabase::MockDatabase>());

    return testDatabase;
}
}  // namespace TestUtility
