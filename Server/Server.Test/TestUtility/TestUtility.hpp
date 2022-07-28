#pragma once

#include <Utility/UtilityTime.hpp>
#include <Network/Connection.hpp>
#include <Models/Primitives.hpp>
#include <Cryptography.hpp>
#include <chrono>

namespace TestUtility
{
using Network::Message;
using LoginInfo        = Models::LoginInfo;
using ReplyInfo        = Models::ReplyInfo;
using Connection       = Network::Connection;
using MessageInfo      = Models::MessageInfo;
using RegistrationInfo = Models::RegistrationInfo;
using RTC              = std::chrono::system_clock;
using milliseconds     = std::chrono::milliseconds;
using Client           = MockClient::MockClient;
using testServer       = Server::Server;
using MessageType      = Message::MessageType;
using Network::SafeQueue;

Message& messageInstance(Message& message, const MessageType messageType)
{
    const std::string testEmail    = "demonstakingoverme@epam.co";
    const std::string testLogin    = "memorisecodead";
    const std::string testPassword = "12juniorgramMargroinuj";
    const std::string testPWDHash  = Base::Hashing::SHA_256(testPassword, testLogin);

    const std::string testChannelName = "testServer";
    const std::string testMessage     = "Hello, juniorgram!!";
    const uint16_t    testReactionID  = 0;
    uint64_t          testChannelID   = 1;
    uint64_t          testMsgID       = 2;
    uint64_t          testReceiverID  = 1;
    uint64_t          testUserID      = 1;

    message.mHeader.mMessageType = messageType;
    message.mHeader.mTimestamp   = RTC::to_time_t(RTC::now());

    switch (messageType)
    {
        case Message::MessageType::RegistrationRequest:
        {
            RegistrationInfo registrationInfo(testEmail, testLogin, testPWDHash);
            message.mBody = std::make_any<RegistrationInfo>(registrationInfo);

            break;
        }

        case Message::MessageType::LoginRequest:
        {
            LoginInfo loginInfo(testLogin, testPWDHash);
            message.mBody = std::make_any<LoginInfo>(loginInfo);

            break;
        }

        case Message::MessageType::ChannelCreateRequest:
        {
            std::string info = testChannelName;
            message.mBody    = std::make_any<std::string>(info);

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
            std::string messageInfo = testChannelName;
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
            MessageInfo messageInfo(testChannelID,testMessage);
            messageInfo._senderID = testUserID;
            messageInfo._msgID    = testMsgID;
            message.mBody           = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::MessageDeleteRequest:
        {
            MessageInfo messageInfo(testChannelID,testMessage);
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
    }
    return message;
}

Client& testSendingMessages(Client& mockClient, const MessageType mesgType)
{
    Message message;

    std::this_thread::sleep_for(milliseconds(1000));
    messageInstance(message, mesgType);
    mockClient.send(message);
    std::this_thread::sleep_for(milliseconds(7000));

    return mockClient;
}

testServer& testServerUpdating(testServer& serverTest)
{
    unsigned int countOfUpdate        = 0;
    unsigned int iterationOfServer    = 1;
    bool         serverWork           = true;

    while (serverWork)
    {
        ++countOfUpdate;
        if (countOfUpdate > iterationOfServer)
        {
            serverWork = false;
            serverTest.stop();
            break;
        }
        std::this_thread::sleep_for(milliseconds(1000));
        serverTest.update();
    }

    return serverTest;
}
}  // namespace TestUtility
