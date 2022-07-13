#pragma once

#include <Utility/UtilityTime.hpp>
#include <Network/Connection.hpp>
#include <Network/Primitives.hpp>
#include <Cryptography.hpp>

namespace TestUtility
{
using Message          = Network::Message;
using LoginInfo        = Network::LoginInfo;
using ReplyInfo        = Network::ReplyInfo;
using Connection       = Network::Connection;
using MessageInfo      = Network::MessageInfo;
using RegistrationInfo = Network::RegistrationInfo;
using RTC              = std::chrono::system_clock;
using Network::SafeQueue;

Message& messageInstance(Message& message,const Message::MessageType& messageType)
{
    const std::string testEmail    = "demonstakingoverme@epam.co";
    const std::string testLogin    = "memorisecodead";
    const std::string testPassword = "12juniorgramMargroinuj";
    const std::string testPWDHash  = Base::Hashing::SHA_256(testPassword, testLogin);

    const std::string testChannelName = "testServer";
    const std::string testMessage     = "Hello, juniorgram!";
    const uint16_t    testReactionID  = 0;
    uint64_t          testChannelID   = 1;
    uint64_t          testMsgID       = 7;
    uint64_t          testReceiverID  = 1;
    uint64_t          testUserID      = 0;

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
            message.mBody = std::make_any<Network::ChannelSubscriptionInfo>(testChannelID);

            break;
        }

        case Message::MessageType::ChannelSubscribeRequest:
        {
            Network::ChannelSubscriptionInfo info;
            info.channelID = testChannelID;
            info.userID    = testUserID;

            message.mBody = std::any_cast<Network::ChannelSubscriptionInfo>(info);

            break;
        }

        case Message::MessageType::ChannelLeaveRequest:
        {
            std::string info = testChannelName;

            message.mBody    = std::make_any<std::string>(info);

            break;
        }

        case Message::MessageType::ReplyHistoryRequest:
        {

            message.mBody = std::make_any<uint64_t>(testChannelID);

            break;
        }

        case Message::MessageType::ReplyStoreRequest:
        {
            ReplyInfo replyInfo;
            replyInfo.channelID = testChannelID;
            replyInfo.message   = testMessage;
            replyInfo.msgID     = testMsgID;

            message.mBody       = std::make_any<ReplyInfo>(replyInfo);

            break;
        }

        case Message::MessageType::MessageReactionRequest:
        {
            MessageInfo messageInfo;
            messageInfo.msgID                     = testMsgID;
            messageInfo.reactions[testReactionID] = std::numeric_limits<uint32_t>::max();

            message.mBody                         = std::make_any<MessageInfo>(messageInfo);

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
            MessageInfo messageInfo;
            messageInfo.message   = testMessage;
            messageInfo.channelID = testChannelID;

            message.mBody         = std::make_any<MessageInfo>(messageInfo);

            break;
        }

        case Message::MessageType::MessageEditRequest:
        {
            MessageInfo info;
            info.msgID     = testMsgID;
            info.channelID = testChannelID;

            message.mBody  = std::make_any<MessageInfo>(info);

            break;
        }

        case Message::MessageType::MessageDeleteRequest:
        {
            MessageInfo info;
            info.channelID = testChannelID;

            message.mBody  = std::make_any<MessageInfo>(info);

            break;
        }

        case Message::MessageType::ChannelDeleteRequest:
        {
            Network::ChannelDeleteInfo channelDeleteInfo;
            channelDeleteInfo.creatorID   = testUserID;
            channelDeleteInfo.channelName = testChannelName;

            message.mBody                 = std::make_any<std::string>(testChannelName);

            break;
        }

        case Message::MessageType::ChannelListRequest:
        {
            std::vector<Network::ChannelInfo> testChannelList;

            message.mBody = std::make_any<std::vector<Network::ChannelInfo>>(testChannelList);

            break;
        }
    }
    return message;
}
}  // namespace TestUtility