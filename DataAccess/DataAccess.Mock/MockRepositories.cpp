#include "MockRepositories.hpp"

namespace MockObject
{
std::vector<Models::ChannelInfo> MockChannelsRepository::getAllChannelsList()
{
    std::vector<std::any> channelsList;
    channelsList.emplace_back(_mockQuery->SelectRepoAndQueryPush("channels",
        TableChannelRepository::ChanneName, std::string("testKernel")));
    if (!channelsList.empty())
    {
        return std::vector<ChannelInfo>(1);
    }
    return std::vector<ChannelInfo>();
}

std::vector<uint64_t> MockChannelsRepository::getChannelSubscriptionList(const uint64_t userID)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("channels",
        TableChannelRepository::UserID, userID); userID > 0)
    {   
        if (_mockQuery->getStorage().getChannelsRepoData()._creatorID == userID)
        {
            return std::vector<uint64_t>(userID);
        }
        return std::vector<uint64_t>();
    }
    return std::vector<uint64_t>();
}

std::vector<Models::MessageInfo> MockMessagesRepository::getMessageHistory(const uint64_t channelID)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("channels", 
        TableChannelRepository::ChannelID, channelID); channelID > 0)
    {
        if (_mockQuery->getStorage().getChannelsRepoData()._channelID == channelID)
        {
            return std::vector<MessageInfo>(channelID);
        }
        return std::vector<MessageInfo>();
    }
    return std::vector<MessageInfo>();
}

std::vector<Models::ReplyInfo> MockRepliesRepository::getReplyHistory(uint64_t channelID)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("replies", 
        TableRepliesRepository::ChannelID, channelID); channelID > 0)
    {
        if (_mockQuery->getStorage().getRepliesRepoData()._channelID == channelID)
        {
            return std::vector<ReplyInfo>(channelID);
        }
        return std::vector<ReplyInfo>();
    }
    return std::vector<ReplyInfo>();
}

Utility::StoringMessageCodes MockMessagesRepository::storeMessage(const MessageInfo& messageInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("msgs",
        TableMessagesRepository::ChannelID, messageInfo._channelID); 
        messageInfo._channelID > 0)
    {
        if (_mockQuery->getStorage().getMessageRepoData()._channelID == messageInfo._channelID)
        {
            return Utility::StoringMessageCodes::SUCCESS;
        }
        return Utility::StoringMessageCodes::FAILED;
    }
    return Utility::StoringMessageCodes::FAILED;
}

Utility::StoringReplyCodes MockRepliesRepository::storeReply(const ReplyInfo& replyInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("replies", 
        TableRepliesRepository::ChannelID, replyInfo._channelID); 
        replyInfo._channelID > 0)
    {
        if (_mockQuery->getStorage().getRepliesRepoData()._channelID == replyInfo._channelID)
        {
            return Utility::StoringReplyCodes::SUCCESS;
        }
        return Utility::StoringReplyCodes::FAILED;
    }
    return Utility::StoringReplyCodes::FAILED;
}

Utility::DeletingMessageCodes MockMessagesRepository::deleteMessage(const MessageInfo& messageInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("msgs", 
        TableMessagesRepository::MessageID, messageInfo._msgID); 
        messageInfo._msgID > 0)
    {
        if (_mockQuery->getStorage().getMessageRepoData()._msgID == messageInfo._msgID)
        {
            return Utility::DeletingMessageCodes::SUCCESS;
        }
        return Utility::DeletingMessageCodes::FAILED;
    }
    return Utility::DeletingMessageCodes::FAILED;
}

Utility::EditingMessageCodes MockMessagesRepository::editMessage(const MessageInfo& messageInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("msgs", 
        TableMessagesRepository::MessageID, messageInfo._msgID); 
        messageInfo._msgID > 0)
    {
        if (_mockQuery->getStorage().getMessageRepoData()._msgID == messageInfo._msgID)
        {
            return Utility::EditingMessageCodes::SUCCESS;
        }
        return Utility::EditingMessageCodes::FAILED;
    }
    return Utility::EditingMessageCodes::FAILED;
}

Utility::ReactionMessageCodes MockMessagesRepository::updateMessageReactions(const MessageInfo& messageInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("msgs",
        TableMessagesRepository::MessageID, messageInfo._msgID);
        messageInfo._msgID > 0)
    {
        if (_mockQuery->getStorage().getMessageRepoData()._msgID == messageInfo._msgID)
        {
            return Utility::ReactionMessageCodes::SUCCESS;
        }
        return Utility::ReactionMessageCodes::FAILED;
    }
    return Utility::ReactionMessageCodes::FAILED;
}

Utility::RegistrationCodes MockRegisterRepository::registerUser(const RegistrationInfo& regInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("user_registration",
        TableRegistrationRepository::UserLogin, regInfo._login);
        regInfo._login != "")
    {
        if (_mockQuery->getStorage().getRegisterRepoData()._login == regInfo._login)
        {
            return Utility::RegistrationCodes::SUCCESS;
        }
        return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
    }
    return Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS;
}

std::uint64_t MockLoginRepository::loginUser(const LoginInfo& loginInfo)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("user_login",
        TableLoginRepository::UserLogin, loginInfo._login);
        loginInfo._login != "")
    {
        if (_mockQuery->getStorage().getLoginRepoData()._login == loginInfo._login)
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

Utility::ChannelDeleteCode MockChannelsRepository::deleteChannel(const ChannelDeleteInfo& channel)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("channels", 
        TableChannelRepository::ChanneName, channel._channelName); 
        channel._channelName != "")
    {
        if (_mockQuery->getStorage().getChannelsRepoData()._channelName == channel._channelName)
        {
            return Utility::ChannelDeleteCode::SUCCESS;
        }
        return Utility::ChannelDeleteCode::FAILED;
    }
    return Utility::ChannelDeleteCode::FAILED;
}

Utility::ChannelCreateCodes MockChannelsRepository::createChannel(const ChannelInfo& channel)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("channels", 
        TableChannelRepository::ChanneName, channel._channelName); 
        channel._channelName != "")
    {
        if (_mockQuery->getStorage().getChannelsRepoData()._channelName == channel._channelName)
        {
            return Utility::ChannelCreateCodes::SUCCESS;
        }
        return Utility::ChannelCreateCodes::FAILED;
    }
    return Utility::ChannelCreateCodes::FAILED;
}

Utility::ChannelLeaveCodes MockChannelsRepository::leaveChannel(const ChannelLeaveInfo& channel)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("channels",
        TableChannelRepository::ChanneName, channel._channelName);
        channel._channelName != "")
    {
        if (_mockQuery->getStorage().getChannelsRepoData()._channelName == channel._channelName)
        {
            return Utility::ChannelLeaveCodes::SUCCESS;
        }
        return Utility::ChannelLeaveCodes::FAILED;
    }
    return Utility::ChannelLeaveCodes::FAILED;
}

Utility::ChannelSubscribingCodes MockChannelsRepository::subscribeToChannel(const ChannelSubscriptionInfo& channel)
{
    if (auto query = _mockQuery->SelectRepoAndQueryPush("channels", 
        TableChannelRepository::UserID, channel._userID); 
         query.has_value() && channel._channelID > 0)
    {
        if(_mockQuery->getStorage().getChannelsRepoData()._creatorID == channel._channelID)
        {
            return Utility::ChannelSubscribingCodes::SUCCESS;
        }
        return Utility::ChannelSubscribingCodes::FAILED;
    }
    return Utility::ChannelSubscribingCodes::FAILED;
}

Utility::DirectMessageStatus MockDirectMessageRepository::addDirectChat(uint64_t userID, uint64_t receiverID)
{
    auto firstQuery = _mockQuery->SelectRepoAndQueryPush("msgs", 
        TableMessagesRepository::UserID, userID);

    if (auto secondQuery = _mockQuery->SelectRepoAndQueryPush("msgs", 
        TableMessagesRepository::RecipientID, receiverID); 
        userID != 0 && receiverID != 0)
    {
        if (_mockQuery->getStorage().getMessageRepoData()._senderID == userID && 
            _mockQuery->getStorage().getMessageRepoData()._recipientID == receiverID)
        {
            return Utility::DirectMessageStatus::SUCCESS;
        }
        return Utility::DirectMessageStatus::FAILED;
    }
    return Utility::DirectMessageStatus::FAILED;
}
}  /// namespace MockObject
