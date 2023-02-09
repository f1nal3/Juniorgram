#pragma once

#include <Models/Primitives.hpp>

#include "MockMessagesOptions.hpp"

namespace TestUtility
{
using LoginInfo               = Models::LoginInfo;
using ReplyInfo               = Models::ReplyInfo;
using MessageInfo             = Models::MessageInfo;
using RegistrationInfo        = Models::RegistrationInfo;
using ChannelSubscriptionInfo = Models::ChannelSubscriptionInfo;
using ChannelDeleteInfo       = Models::ChannelDeleteInfo;
using ChannelLeaveInfo        = Models::ChannelLeaveInfo;
using ChannelInfo             = Models::ChannelInfo;

/**
* @brief MessageFiller class.
* @details This class is for filling the body of messages as well as taking them. /
*          We can fill the message individually or all at once.
*/
class MessageFiller
{
public:
    MessageFiller()  = default;
    ~MessageFiller() = default;

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
    void                    setLoginInfo(const LoginInfo& logInfo);
    void                    setRegistrationInfo(const RegistrationInfo& regInfo);
    void                    setChannelInfo(const ChannelInfo& chanInfo);
    void                    setChannelLeaveInfo(const ChannelLeaveInfo& chanInfo);
    void                    setChannelDeleteInfo(const ChannelDeleteInfo& chanInfo);
    void                    setChannelSubscriptionInfo(const ChannelSubscriptionInfo& chanInfo);
    void                    setReplyInfo(const ReplyInfo& repInfo);
    void                    setMessageInfo(const MessageInfo& mesgInfo);

    LoginInfo               _loginInfo;
    RegistrationInfo        _regInfo;
    ChannelInfo             _channelInfo;
    ChannelLeaveInfo        _channelLeaveInfo;
    ChannelDeleteInfo       _channelDelInfo;
    ChannelSubscriptionInfo _channelSubInfo;
    ReplyInfo               _replyInfo;
    MessageInfo             _messageInfo;
};
}  // namespace TestUtility
