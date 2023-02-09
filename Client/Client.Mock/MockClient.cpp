#include "MockClient.hpp"

namespace MockObject
{
using MessageType = Network::Message::MessageType;
using UtilityTime::timestamp_t;
using UtilityTime::RTC;

void MockClient::onLoginAnswer(bool success)
{
    if (success)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] The user is logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] User is not logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onServerAccepted()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] Server accepted the connection!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onServerPing(double timestamp)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onServerMessage(const uint64_t clientID)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] Hello from [" + std::to_string(clientID) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onChannelListRequest(const std::vector<Models::ChannelInfo>& channels)
{
    if (!channels.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel list sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel list isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages)
{
    if (messages.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message history isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message history is taken successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
{
    if (storingMessageCode == Utility::StoringMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
         Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Message didn't sent!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
{
   
    if (registrationCode == Utility::RegistrationCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] A user has registered!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] The user already exists!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState)
{
    if (deletingState == Utility::DeletingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionState)
{
    if (reactionState == Utility::ReactionMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reaction updating success!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reaction updating failed", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const
{
    if (reactionState == Utility::EditingMessageCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success editing message!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Bad editing message!",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onDisconnect()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[MockClient] You've been disconnected from server!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onMessageSendFailed(const Message& message) const
{
    (void)(message);
    Base::Logger::FileLogger::getInstance().log
    (   
        "[MockClient] Message send failed is not implemented!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies)
{
    if (!replies.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reply history answer is implemented!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Reply history answer isn't implemented!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode)
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success sending!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed sending!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode)
{
    if (ChannelLeaveCode == Utility::ChannelLeaveCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success leaving!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed leaving!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode)
{
    if (subscribingChannelCode == Utility::ChannelSubscribingCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success subscribing",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed subscribing", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList)
{
    if (!subscribingChannelList.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel subscribing list is successfully!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Channel subscribing list isn't implementet!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode)
{
    if (channelDeleteCode == Utility::ChannelDeleteCode::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success deleting!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed deleting!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode)
{
    if (channelCreateCode == Utility::ChannelCreateCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Faild creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer)
{
    if (directMessageCreateAnswer == Utility::DirectMessageStatus::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Success creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockClient] Failed creating!", 
            Base::Logger::LogLevel::INFO
        );
    }
}
}  /// namespace MockObject
