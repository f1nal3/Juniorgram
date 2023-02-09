#include "MockRepositories.hpp"

namespace MockObject
{
std::vector<Models::ChannelInfo> MockChannelsRepository::getAllChannelsList()
{
    std::vector<std::any> channelsList;
    channelsList.emplace_back(_mockQuery->getAdapter()->query("channels"));
    if (!channelsList.empty())
    {
        Base::Logger::FileLogger::getInstance().log
        (
             "[MockRepositories]: All channels are successfully given to client!\n",
            Base::Logger::LogLevel::INFO
        );
        return std::vector<Models::ChannelInfo>(1);
    }
    return std::vector<Models::ChannelInfo>();
}

std::vector<uint64_t> MockChannelsRepository::getChannelSubscriptionList(const uint64_t userID)
{
    if (auto query = _mockQuery->SelectAndPushData("channels", std::to_string(userID));
        query.has_value() && userID > 0)
    {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: List of subscriptions of channel id: "
             + std::to_string(userID)
             + "] was successfully given!\n",
             Base::Logger::LogLevel::INFO
         );
        return std::vector<uint64_t>(userID);
    }
     return std::vector<uint64_t>();
}

std::vector<Models::MessageInfo> MockMessagesRepository::getMessageHistory(const std::uint64_t channelID)
{
     if (auto query = _mockQuery->SelectAndPushData("channels", std::to_string(channelID));
         query.has_value() && channelID > 0)
     {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [channel id: " + std::to_string(channelID)
            + "]: Message history was successfully given to client!\n",
            Base::Logger::LogLevel::INFO
        );
        return std::vector<Models::MessageInfo>(channelID);
     }
     return std::vector<Models::MessageInfo>();
}

std::vector<Models::ReplyInfo> MockRepliesRepository::getReplyHistory(std::uint64_t channelID)
{
     if (auto query = _mockQuery->SelectAndPushData("channels", std::to_string(channelID));
         query.has_value() && channelID > 0)
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: Reply history was successfully given!\n",
             Base::Logger::LogLevel::INFO
         );
         return std::vector<Models::ReplyInfo>(channelID);
     }
     return std::vector<Models::ReplyInfo>();
}

Utility::StoringMessageCodes MockMessagesRepository::storeMessage(const Models::MessageInfo& messageInfo)
{
     if (auto query = _mockQuery->SelectAndPushData("msgs", std::to_string(messageInfo._channelID));
         query.has_value() && messageInfo._channelID > 0)
     {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [message ID: " + std::to_string(messageInfo._msgID)
            + "]: Message have stored successfully!\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::StoringMessageCodes::SUCCESS;
     }
     return Utility::StoringMessageCodes::FAILED;
}

Utility::StoringReplyCodes MockRepliesRepository::storeReply(const Models::ReplyInfo& replyInfo)
{
    if (auto query = _mockQuery->SelectAndPushData("replies", std::to_string(replyInfo._channelID));
        query.has_value() && replyInfo._channelID > 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [channel ID: " + std::to_string(replyInfo._channelID)
            + "] [message: " + replyInfo._message
            + "]: Reply successfully stored in database!\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::StoringReplyCodes::SUCCESS;
    }
    return Utility::StoringReplyCodes::FAILED;
}

Utility::DeletingMessageCodes MockMessagesRepository::deleteMessage(const Models::MessageInfo& messageInfo)
{
    if (auto query = _mockQuery->SelectAndPushData("msgs", std::to_string(messageInfo._msgID));
        query.has_value() && messageInfo._msgID > 0)
   {
       Base::Logger::FileLogger::getInstance().log
       (
           "[MockRepositories]: Message have deleted successfully!\n",
           Base::Logger::LogLevel::INFO
       );
       return Utility::DeletingMessageCodes::SUCCESS;
   }
   return Utility::DeletingMessageCodes::FAILED;
}

Utility::EditingMessageCodes MockMessagesRepository::editMessage(const Models::MessageInfo& messageInfo)
{
   if (auto query = _mockQuery->SelectAndPushData("msgs", std::to_string(messageInfo._msgID));
       query.has_value() && messageInfo._msgID > 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Message have edited successfully!\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::EditingMessageCodes::SUCCESS;
    }
    return Utility::EditingMessageCodes::FAILED;
}

Utility::ReactionMessageCodes MockMessagesRepository::updateMessageReactions(const Models::MessageInfo& messageInfo)
{
    if (auto query = _mockQuery->SelectAndPushData("msgs", std::to_string(messageInfo._msgID)); 
         query.has_value() && messageInfo._msgID > 0)
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: Message reactions updated!\n",
             Base::Logger::LogLevel::INFO
         );
         return Utility::ReactionMessageCodes::SUCCESS;
     }
     return Utility::ReactionMessageCodes::FAILED;
}

Utility::RegistrationCodes MockRegisterRepository::registerUser(const Models::RegistrationInfo& regInfo)
{
     if (auto query = _mockQuery->SelectAndPushData("user_registration", regInfo._login); 
         query.has_value() && regInfo._login != "")
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: User successfully registered!\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::RegistrationCodes::SUCCESS;
    }
    return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
}

std::uint64_t MockLoginRepository::loginUser(const Models::LoginInfo& loginInfo)
{
    if (auto query = _mockQuery->SelectAndPushData("user_login", loginInfo._login); 
         query.has_value() && loginInfo._login != "")
    {
       Base::Logger::FileLogger::getInstance().log
       (
           "[MockRepositories]: User successfully logging!\n",
           Base::Logger::LogLevel::INFO
       );
       return 1;
    }
    return 0;
}

Utility::ChannelDeleteCode MockChannelsRepository::deleteChannel(const Models::ChannelDeleteInfo& channel)
{
    if (auto query = _mockQuery->SelectAndPushData("channels", channel._channelName); 
         query.has_value() && channel._channelName != "")
    {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: [channel name: " + channel._channelName
             + "]: Deleting channel was successful!\n",
             Base::Logger::LogLevel::INFO
         );
         return Utility::ChannelDeleteCode::SUCCESS;
    }
    return Utility::ChannelDeleteCode::FAILED;
}

Utility::ChannelCreateCodes MockChannelsRepository::createChannel(const Models::ChannelInfo& channel)
{
     if (auto query = _mockQuery->SelectAndPushData("channels", channel._channelName); 
         query.has_value() && channel._channelName != "")
     {
          Base::Logger::FileLogger::getInstance().log
          (
              "[MockRepositories]: [channel name: " + channel._channelName
              + "]: Creating channel was successful!\n",
              Base::Logger::LogLevel::INFO
          );
          return Utility::ChannelCreateCodes::SUCCESS;
     }
     return Utility::ChannelCreateCodes::FAILED;
}

Utility::ChannelLeaveCodes MockChannelsRepository::leaveChannel(const Models::ChannelLeaveInfo& channel)
{
    if (auto query = _mockQuery->SelectAndPushData("channels", channel._channelName); 
         query.has_value() && channel._channelName != "")
    {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: [channel name: " + channel._channelName
             + "]: Leaving from channel is successful!\n",
             Base::Logger::LogLevel::INFO
         );
         return Utility::ChannelLeaveCodes::SUCCESS;
    }
    return Utility::ChannelLeaveCodes::FAILED;
}

Utility::ChannelSubscribingCodes MockChannelsRepository::subscribeToChannel(const Models::ChannelSubscriptionInfo& channel)
{
     if (auto query = _mockQuery->SelectAndPushData("channels", std::to_string(channel._userID)); 
         query.has_value() && channel._channelID > 0)
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: [User ID: " + std::to_string(channel._userID)
             + "]: User successfully subscribed to the channel!\n",
             Base::Logger::LogLevel::INFO
         );
         return Utility::ChannelSubscribingCodes::SUCCESS;
     }
     return Utility::ChannelSubscribingCodes::FAILED;
}

Utility::DirectMessageStatus MockDirectMessageRepository::addDirectChat(uint64_t userID, uint64_t receiverID)
{
     auto firstQuery = _mockQuery->SelectAndPushData("direct_msgs", std::to_string(userID));

    if (auto secondQuery = _mockQuery->SelectAndPushData("direct_msgs", std::to_string(receiverID)); 
        firstQuery.has_value() && userID != 0 && secondQuery.has_value() && receiverID != 0)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Direct chat successfully created!\n", 
            Base::Logger::LogLevel::INFO
        );
        return Utility::DirectMessageStatus::SUCCESS;
    }
    return Utility::DirectMessageStatus::FAILED;
}
}  /// namespace MockObject
