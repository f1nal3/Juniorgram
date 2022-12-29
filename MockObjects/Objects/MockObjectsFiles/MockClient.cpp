#include "MockClient.hpp"

namespace MockClient
{
using MessageType = Network::Message::MessageType;
using UtilityTime::timestamp_t;
using UtilityTime::RTC;

void MockClient::onLoginAnswer(bool success)
{
    enum class LoginState
    {
        SUCCESS,
        IN_PROGRESS,
        FAILED
    };

    static LoginState loginState;

    if (success)
    {
        loginState = LoginState::SUCCESS;

        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] The user is logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        loginState = LoginState::FAILED;

        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] User is not logged in!", 
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onServerAccepted()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Server accepted the connection!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onServerPing(double timestamp)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Ping: " + std::to_string(timestamp), 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onServerMessage(const uint64_t clientID)
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Hello from [" + std::to_string(clientID) + "]", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onChannelListRequest(const std::vector<Models::ChannelInfo>& channels)
{
    if (!channels.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel list sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel list isn't implemented!",
            Base::Logger::LogLevel::INFO
        );
     }
}

void MockClient::onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages)
{
    if (!messages.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message history is successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message history isn't implemented!",
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
            "[CLIENT] Message sent successfully!",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (storingMessageCode == Utility::StoringMessageCodes::FAILED)
    {
         Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Message not sent!:(!", 
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
            "[CLIENT] A user has registered!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] The user already exists", 
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
            "[CLIENT] Success deleting", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (deletingState == Utility::DeletingMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Failed deleting", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] UNKNOWN deleting message code", 
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
            "[CLIENT] Reaction update success", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (reactionState == Utility::ReactionMessageCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Reaction update FAILED", 
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
            "[CLIENT] Success editing message",
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Bad editing message",
            Base::Logger::LogLevel::INFO
        );
    }
}

void MockClient::onDisconnect()
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] You've been disconnected from server!", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onMessageSendFailed(const Message& message) const
{
    Base::Logger::FileLogger::getInstance().log
    (
        "[CLIENT] Message send failed is not implemented", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies)
{
    (void)(replies);
    Base::Logger::FileLogger::getInstance().log
    (
        "Reply history answer is implemented", 
        Base::Logger::LogLevel::INFO
    );
}

void MockClient::onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode)
{
    if (storingReplyCode == Utility::StoringReplyCodes::SUCCESS)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Success sending", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (storingReplyCode == Utility::StoringReplyCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Failed sending", 
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
            "[CLIENT] Success leaving",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Failed leaving", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (ChannelLeaveCode == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel not found", 
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
            "[CLIENT] Success subscribing",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (subscribingChannelCode == Utility::ChannelSubscribingCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Failed subscribing", 
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
            "[CLIENT] Channel subscribing list is successfully!!", 
            Base::Logger::LogLevel::INFO
        );
    }
    else
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel subscribing list isn't implementet!!", 
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
            "[CLIENT] Success deleting",
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Failed deleting", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel not found", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelDeleteCode == Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel doesn't belong to the user", 
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
            "[CLIENT] Success creating", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Faild creating", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (channelCreateCode == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Channel already create",
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
            "[CLIENT] Success creating", 
            Base::Logger::LogLevel::INFO
        );
    }
    else if (directMessageCreateAnswer == Utility::DirectMessageStatus::FAILED)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[CLIENT] Failed creating", 
            Base::Logger::LogLevel::INFO
        );
    }
}
}  // namespace MockClient
