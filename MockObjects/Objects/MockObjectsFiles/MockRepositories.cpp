#include "MockRepositories.hpp"

#include "DataAccess.Postgre/UsersAmountFinder.hpp"
#include "Network/Connection.hpp"
#include "Models/Primitives.hpp"

#include "Cryptography.hpp"
#include "FileLogger.hpp"

namespace MockRepositories
{
std::vector<Models::ChannelInfo> testChannelsRepository::getAllChannelsList()
{
    Models::ChannelInfo channelInfo;

    channelInfo._channelID   = 15;
    channelInfo._channelName = "testServer";
    channelInfo._creatorID   = 1;

    std::vector<Models::ChannelInfo> result;
    result.push_back(channelInfo);

    Base::Logger::FileLogger::getInstance().log
    (
        "[MockRepositories]: All channels are successfully given to client\n",
        Base::Logger::LogLevel::INFO
    );
    return result;
}

std::vector<uint64_t> testChannelsRepository::getChannelSubscriptionList(const uint64_t userID)
{
    constexpr uint64_t    testUserID = 1;

    std::vector<uint64_t> result;

    if (userID == testUserID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: List of subscriptions of channel id: " 
            + std::to_string(userID)
            + " was successfully given\n",
            Base::Logger::LogLevel::INFO
        );
        result.push_back(testUserID);
    }
    return result;
}

std::vector<Models::MessageInfo> testMessagesRepository::getMessageHistory(const std::uint64_t channelID)
{
    std::vector<Models::MessageInfo> result;

    Models::MessageInfo messageInfo;
    messageInfo._msgID = 1;
    messageInfo._senderID = 1;
    messageInfo._message  = "Hello, Juniorgram!!";
    messageInfo._userLogin    = "memorisecodead";
    messageInfo._reactions[4] = {1};

    if (channelID == messageInfo._channelID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [channel id: " + std::to_string(channelID) 
            + "]: Message history was successfully given to client\n",
            Base::Logger::LogLevel::INFO
        );
        result.push_back(messageInfo);
    }
    return result;
}

std::vector<Models::ReplyInfo> testRepliesRepository::getReplyHistory(std::uint64_t channelID)
{
    std::vector<Models::ReplyInfo> result;

    Models::ReplyInfo replyInfo;
    replyInfo._channelID = 1;

    if (channelID == replyInfo._channelID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Reply history was successfully given\n",
            Base::Logger::LogLevel::INFO
        );
        result.emplace_back(replyInfo);
    }
    return result;
}

Utility::StoringMessageCodes testMessagesRepository::storeMessage(const Models::MessageInfo& messageInfo)
{
    Models::MessageInfo testMessageInfo;
    testMessageInfo._channelID = 1;

    if (messageInfo._channelID == testMessageInfo._channelID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [creator ID: " + std::to_string(messageInfo._channelID) 
            + "] [message ID: " + std::to_string(messageInfo._msgID) 
            + "]: Message have stored successfully\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::StoringMessageCodes::SUCCESS;
    }
    return Utility::StoringMessageCodes::FAILED;
}

Utility::StoringReplyCodes testRepliesRepository::storeReply(const Models::ReplyInfo& replyInfo)
{
    Models::ReplyInfo testReplyInfo;
    testReplyInfo._channelID = 1;

    if (replyInfo._channelID == testReplyInfo._channelID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [channel ID: " + std::to_string(replyInfo._channelID) 
            + "] [message: " + replyInfo._message 
            + "]: Reply successfully stored in database\n", 
            Base::Logger::LogLevel::INFO
        );
        return Utility::StoringReplyCodes::SUCCESS;
    }
    return Utility::StoringReplyCodes::FAILED;
}

Utility::DeletingMessageCodes testMessagesRepository::deleteMessage(const Models::MessageInfo& messageInfo)
{
    Models::MessageInfo testMessageInfo;
    testMessageInfo._msgID = 0;

    if (messageInfo._msgID == testMessageInfo._msgID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Message have deleted successfully\n", 
            Base::Logger::LogLevel::INFO
        );
        return Utility::DeletingMessageCodes::SUCCESS;
    }
    return Utility::DeletingMessageCodes::FAILED;
}

Utility::EditingMessageCodes testMessagesRepository::editMessage(const Models::MessageInfo& messageInfo)
{
    Models::MessageInfo mesgInfo;
    mesgInfo._msgID    = 2;

    if (mesgInfo._msgID == messageInfo._msgID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Message have edited successfully\n", 
            Base::Logger::LogLevel::INFO
        );
        return Utility::EditingMessageCodes::SUCCESS;
    }
    return Utility::EditingMessageCodes::FAILED;
}

Utility::ReactionMessageCodes testMessagesRepository::updateMessageReactions(const Models::MessageInfo& messageInfo)
{
    Models::MessageInfo testMessageInfo;
    testMessageInfo._msgID = 2;

    if (testMessageInfo._msgID == messageInfo._msgID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Message reactions updated\n", 
            Base::Logger::LogLevel::INFO
        );

        return Utility::ReactionMessageCodes::SUCCESS;
    }
    return Utility::ReactionMessageCodes::FAILED;
}

Utility::RegistrationCodes testRegisterRepository::registerUser(const Models::RegistrationInfo& regInfo)
{
    Models::RegistrationInfo testRegInfo;
    testRegInfo._login = "memorisecodead";

    if (testRegInfo._login == regInfo._login)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: User successfully registered\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::RegistrationCodes::SUCCESS;
    }
    return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
}

std::uint64_t testLoginRepository::loginUser(const Models::LoginInfo& loginInfo)
{ 
    Models::LoginInfo testLogInfo;
    testLogInfo._login = "memorisecodead";

    if (testLogInfo._login == loginInfo._login)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: User successfully logging\n",
            Base::Logger::LogLevel::INFO
        );
        return 1;
    }
    return 0;
}

Utility::ChannelDeleteCode testChannelsRepository::deleteChannel(const Models::ChannelDeleteInfo& channel)
{
    Models::ChannelDeleteInfo testChannelInfo;
    testChannelInfo._channelName = "testServer";

   if (testChannelInfo._channelName == channel._channelName)
   {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [creator ID: " + std::to_string(channel._creatorID) 
            + "] [channel name: " + channel._channelName 
            + "]: Deleting channel was successful\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::ChannelDeleteCode::SUCCESS;
   }
   return Utility::ChannelDeleteCode::FAILED;
}

Utility::ChannelCreateCodes testChannelsRepository::createChannel(const Models::ChannelInfo& channel)
{
   Models::ChannelInfo testChannelInfo;
   testChannelInfo._channelName = "testServer";

   if (testChannelInfo._channelName == channel._channelName)
   {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [creator ID: " + std::to_string(channel._creatorID) 
            + "] [channel name: " + channel._channelName 
            + "]: Creating channel was successful\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::ChannelCreateCodes::SUCCESS;
   }
   return Utility::ChannelCreateCodes::FAILED;
}

Utility::ChannelLeaveCodes testChannelsRepository::leaveChannel(const Models::ChannelLeaveInfo& channel)
{
    Models::ChannelLeaveInfo testChannelInfo;
    testChannelInfo._channelName = "testServer";

    if (testChannelInfo._channelName == channel._channelName)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [creator ID: " + std::to_string(channel._creatorID) 
            + "] [channel name: " + channel._channelName 
            + "]: Leaving from channel is successful\n",
            Base::Logger::LogLevel::INFO
        );
        return Utility::ChannelLeaveCodes::SUCCESS;
    }
    return Utility::ChannelLeaveCodes::FAILED;
}

Utility::ChannelSubscribingCodes testChannelsRepository::subscribeToChannel(const Models::ChannelSubscriptionInfo& channel)
{
    Models::ChannelSubscriptionInfo testChannelInfo;
    testChannelInfo._channelID = 1;

    if (testChannelInfo._channelID == channel._channelID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: [creator ID: " + std::to_string(channel._channelID) 
            + "] [User ID: " + std::to_string(channel._userID) 
            + "]: User successfully subscribed to the channel\n",
            Base::Logger::LogLevel::INFO
        );

        return Utility::ChannelSubscribingCodes::SUCCESS;
    }

    return Utility::ChannelSubscribingCodes::FAILED;
}

Utility::DirectMessageStatus testDirectMessageRepository::addDirectChat(uint64_t userID, uint64_t receiverID)
{
    constexpr uint64_t testUserID = 1;

    if (constexpr uint64_t testReceiverID = 1;
        testUserID == userID && receiverID == testReceiverID)
    {
        Base::Logger::FileLogger::getInstance().log
        (
            "[MockRepositories]: Direct chat successfully created\n", 
            Base::Logger::LogLevel::INFO
        );
        return Utility::DirectMessageStatus::SUCCESS;
    }
    return Utility::DirectMessageStatus::FAILED;
}
}  // namespace MockRepositories
