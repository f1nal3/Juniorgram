#pragma once

#include <Models/Primitives.hpp>

#include "TestMessagesOptions.hpp"

namespace MesgFiller
{
using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelSubscriptionInfo = Models::ChannelSubscriptionInfo;
using ChannelDeleteInfo       = Models::ChannelDeleteInfo;
using ChannelLeaveInfo        = Models::ChannelLeaveInfo;
using ChannelInfo             = Models::ChannelInfo;

class MessageFiller
{
public:
    MessageFiller()  = default;
    ~MessageFiller() = default;

    void setLoginInfo(LoginInfo& logInfo);
    void setRegistrationInfo(RegistrationInfo& regInfo);
    void setChannelInfo(ChannelInfo& chanInfo);
    void setChannelLeaveInfo(ChannelLeaveInfo& chanInfo);
    void setChannelDeleteInfo(ChannelDeleteInfo& chanInfo);
    void setChannelSubscriptionInfo(ChannelSubscriptionInfo& chanInfo); 
    void setReplyInfo(ReplyInfo& repInfo);
    void setMessageInfo(MessageInfo& mesgInfo);

    LoginInfo&               getLoginInfo();
    RegistrationInfo&        getRegistrationInfo();
    ChannelInfo&             getChannelInfo();
    ChannelLeaveInfo&        getChannelLeaveInfo();
    ChannelDeleteInfo&       getChannelDeleteInfo();
    ChannelSubscriptionInfo& getChannelSubscriptionInfo();
    ReplyInfo&               getReplyInfo();
    MessageInfo&             getMessageInfo();

    void fillLoginInfo();
    void fillRegistrationInfo();
    void fillChannelInfo();
    void fillChannelLeaveInfo();
    void fillChannelDeleteInfo();
    void fillChannelSubscriptionInfo();
    void fillReplyInfo();
    void fillMessageInfo();
    void fillAllMessages();

private:
    LoginInfo               _loginInfo;
    RegistrationInfo        _regInfo;
    ChannelInfo             _channelInfo;
    ChannelLeaveInfo        _channelLeaveInfo;
    ChannelDeleteInfo       _channelDelInfo;
    ChannelSubscriptionInfo _channelSubInfo;
    ReplyInfo               _replyInfo;
    MessageInfo             _messageInfo;
};
}  // namespace MesgFiller
