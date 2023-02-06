#include "MockRepositories.hpp"

namespace MockRepositories
{
std::vector<Models::ChannelInfo> MockChannelsRepository::getAllChannelsList()
{
    Models::ChannelInfo channelInfo;
    auto query = _mockQuery->getQuery()->query(channelInfo._channelName);

    if (query.has_value())
    {
        
        Base::Logger::FileLogger::getInstance().log
        (
             "[MockRepositories]: All channels are successfully given to client!\n",
            Base::Logger::LogLevel::INFO
        );
    }
    return std::vector<Models::ChannelInfo>();
}

std::vector<uint64_t> MockChannelsRepository::getChannelSubscriptionList(const uint64_t userID)
{
    auto query = _mockQuery->getQuery()->query(std::to_string(userID));

     if (query.has_value())
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: List of subscriptions of channel id: "
             + std::to_string(userID)
             + "] was successfully given!\n",
             Base::Logger::LogLevel::INFO
         );
     }
     return std::vector<uint64_t>();
}

std::vector<Models::MessageInfo> MockMessagesRepository::getMessageHistory(const std::uint64_t channelID)
{
    auto query = _mockQuery->getQuery()->query(std::to_string(channelID));

    if (query.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [channel id: " + std::to_string(channelID)
            + "]: Message history was successfully given to client!\n",
            Base::Logger::LogLevel::INFO
        );
    }
    return std::vector<Models::MessageInfo>();
}

std::vector<Models::ReplyInfo> MockRepliesRepository::getReplyHistory(std::uint64_t channelID)
{
    auto query = _mockQuery->getQuery()->query(std::to_string(channelID));

     if (query.has_value())
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: Reply history was successfully given!\n",
             Base::Logger::LogLevel::INFO
         );
     }
     return std::vector<Models::ReplyInfo>();
}

Utility::StoringMessageCodes MockMessagesRepository::storeMessage(const Models::MessageInfo& messageInfo)
{
    auto query = _mockQuery->getQuery()->query(std::to_string(messageInfo._channelID));

    if (query.has_value())
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [creator ID: " + std::to_string(messageInfo._channelID)
            + "] [message ID: " + std::to_string(messageInfo._msgID)
            + "]: Message have stored successfully!\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::StoringMessageCodes::SUCCESS;
    }
    return Utility::StoringMessageCodes::FAILED;
}

Utility::StoringReplyCodes MockRepliesRepository::storeReply(const Models::ReplyInfo& replyInfo)
{
    auto query = _mockQuery->getQuery()->query(std::to_string(replyInfo._channelID));

    if (query.has_value())
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
   auto query = _mockQuery->getQuery()->query(std::to_string(messageInfo._msgID));

   if (query.has_value())
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
    auto query = _mockQuery->getQuery()->query(std::to_string(messageInfo._msgID));

    if (query.has_value())
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
    auto query = _mockQuery->getQuery()->query(std::to_string(messageInfo._msgID));

     if (query.has_value())
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
    auto query = _mockQuery->getQuery()->query(regInfo._login);

    if (query.has_value())
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
    auto query = _mockQuery->getQuery()->query(loginInfo._login);

   if (query.has_value())
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
    auto query = _mockQuery->getQuery()->query(channel._channelName);

     if (query.has_value())
    {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: [creator ID: " + std::to_string(channel._creatorID)
             + "] [channel name: " + channel._channelName
             + "]: Deleting channel was successful!\n",
             Base::Logger::LogLevel::INFO
         );
         return Utility::ChannelDeleteCode::SUCCESS;
    }
    return Utility::ChannelDeleteCode::FAILED;
}

Utility::ChannelCreateCodes MockChannelsRepository::createChannel(const Models::ChannelInfo& channel)
{
    auto query = _mockQuery->getQuery()->query(channel._channelName);

     if (query.has_value())
     {
          Base::Logger::FileLogger::getInstance().log
          (
              "[MockRepositories]: [creator ID: " + std::to_string(channel._creatorID)
              + "] [channel name: " + channel._channelName
              + "]: Creating channel was successful!\n",
              Base::Logger::LogLevel::INFO
          );
          return Utility::ChannelCreateCodes::SUCCESS;
     }
     return Utility::ChannelCreateCodes::FAILED;
}

Utility::ChannelLeaveCodes MockChannelsRepository::leaveChannel(const Models::ChannelLeaveInfo& channel)
{
    auto query = _mockQuery->getQuery()->query(channel._channelName);

    if (query.has_value())
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: [creator ID: " + std::to_string(channel._creatorID)
             + "] [channel name: " + channel._channelName
             + "]: Leaving from channel is successful!\n",
             Base::Logger::LogLevel::INFO
         );
         return Utility::ChannelLeaveCodes::SUCCESS;
     }
     return Utility::ChannelLeaveCodes::FAILED;
}

Utility::ChannelSubscribingCodes MockChannelsRepository::subscribeToChannel(const Models::ChannelSubscriptionInfo& channel)
{
     auto query = _mockQuery->getQuery()->query(std::to_string(channel._channelID));

     if (query.has_value())
     {
         Base::Logger::FileLogger::getInstance().log
         (
             "[MockRepositories]: [creator ID: " + std::to_string(channel._channelID)
             + "] [User ID: " + std::to_string(channel._userID)
             + "]: User successfully subscribed to the channel!\n",
             Base::Logger::LogLevel::INFO
         );

         return Utility::ChannelSubscribingCodes::SUCCESS;
     }
     return Utility::ChannelSubscribingCodes::FAILED;
}

Utility::DirectMessageStatus MockDirectMessageRepository::addDirectChat(uint64_t userID, uint64_t receiverID)
{
    auto firstQuery = _mockQuery->getQuery()->query(std::to_string(userID));
    auto secondQuery = _mockQuery->getQuery()->query(std::to_string(receiverID)); 

    if (firstQuery.has_value() && secondQuery.has_value())
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
}  // namespace MockRepositories
