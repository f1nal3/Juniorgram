#include "MessageFiller.hpp"

namespace TestUtility
{
void MessageFiller::setLoginInfo(const LoginInfo& logInfo)
{
    _loginInfo._login   = logInfo._login;
    _loginInfo._pwdHash = logInfo._pwdHash;
}

void MessageFiller::setRegistrationInfo(const RegistrationInfo& regInfo)
{
    _regInfo._login        = regInfo._login;
    _regInfo._email        = regInfo._email;
    _regInfo._passwordHash = regInfo._passwordHash;
}

void MessageFiller::setChannelInfo(const ChannelInfo& chanInfo)
{
    _channelInfo._channelID   = chanInfo._channelID;
    _channelInfo._channelName = chanInfo._channelName;
    _channelInfo._creatorID   = chanInfo._creatorID;
}

void MessageFiller::setChannelLeaveInfo(const ChannelLeaveInfo& chanInfo)
{
    _channelLeaveInfo._channelID   = chanInfo._channelID;
    _channelLeaveInfo._channelName = chanInfo._channelName;
    _channelLeaveInfo._creatorID   = chanInfo._creatorID;
}

void MessageFiller::setChannelDeleteInfo(const ChannelDeleteInfo& chanInfo)
{
    _channelDelInfo._channelID   = chanInfo._channelID;
    _channelDelInfo._channelName = chanInfo._channelName;
    _channelDelInfo._creatorID   = chanInfo._creatorID;
}

void MessageFiller::setChannelSubscriptionInfo(const ChannelSubscriptionInfo& chanInfo)
{
    _channelSubInfo._channelID = chanInfo._channelID;
    _channelSubInfo._userID    = chanInfo._userID;
}

void MessageFiller::setReplyInfo(const ReplyInfo& repInfo)
{
    _replyInfo._channelID  = repInfo._channelID;
    _replyInfo._message    = repInfo._message;
    _replyInfo._msgID      = repInfo._msgID;
    _replyInfo._msgIdOwner = repInfo._msgIdOwner;
    _replyInfo._senderID   = repInfo._senderID;
    _replyInfo._userLogin  = repInfo._userLogin;
}

void MessageFiller::setMessageInfo(const MessageInfo& mesgInfo)
{
    _messageInfo._channelID   = mesgInfo._channelID;
    _messageInfo._message     = mesgInfo._message;
    _messageInfo._msgID       = mesgInfo._msgID;
    _messageInfo._reactions   = mesgInfo._reactions;
    _messageInfo._recipientID = mesgInfo._recipientID;
    _messageInfo._senderID    = mesgInfo._senderID;
    _messageInfo._time        = mesgInfo._time;
    _messageInfo._userLogin   = mesgInfo._userLogin;
}

LoginInfo& MessageFiller::getLoginInfo()
{
    return _loginInfo; 
}

RegistrationInfo& MessageFiller::getRegistrationInfo()
{
    return _regInfo;
}

ChannelInfo& MessageFiller::getChannelInfo()
{
    return _channelInfo;
}

ChannelLeaveInfo& MessageFiller::getChannelLeaveInfo()
{
    return _channelLeaveInfo;
}

ChannelDeleteInfo& MessageFiller::getChannelDeleteInfo()
{
    return _channelDelInfo;
}

ChannelSubscriptionInfo& MessageFiller::getChannelSubscriptionInfo()
{
    return _channelSubInfo;
}

ReplyInfo& MessageFiller::getReplyInfo()
{
    return _replyInfo;
}

MessageInfo& MessageFiller::getMessageInfo()
{
    return _messageInfo;
}

void MessageFiller::fillLoginInfo()
{
    LoginInfo loginInfo;
    loginInfo._login   = testLogin;
    loginInfo._pwdHash = testPWDHash;

    setLoginInfo(loginInfo);
}

void MessageFiller::fillRegistrationInfo()
{
    RegistrationInfo regInfo;
    regInfo._email        = testEmail;
    regInfo._login        = testLogin;
    regInfo._passwordHash = testPWDHash;

    setRegistrationInfo(regInfo);
}
void MessageFiller::fillChannelInfo()
{
    ChannelInfo channelInfo;
    channelInfo._channelID   = testChannelID;
    channelInfo._channelName = testChannelName;
    channelInfo._creatorID   = testCreatorID;

    setChannelInfo(channelInfo);
}
void MessageFiller::fillChannelLeaveInfo()
{
    ChannelLeaveInfo leaveInfo;
    leaveInfo._channelID   = testChannelID;
    leaveInfo._channelName = testChannelName;
    leaveInfo._creatorID   = testCreatorID;

    setChannelLeaveInfo(leaveInfo);
}
void MessageFiller::fillChannelDeleteInfo()
{
    ChannelDeleteInfo deleteInfo;
    deleteInfo._channelID   = testChannelID;
    deleteInfo._channelName = testChannelName;
    deleteInfo._creatorID   = testCreatorID;

    setChannelDeleteInfo(deleteInfo);
}
void MessageFiller::fillChannelSubscriptionInfo()
{
    ChannelSubscriptionInfo subInfo;
    subInfo._channelID = testChannelID;
    subInfo._userID    = testUserID;

    setChannelSubscriptionInfo(subInfo);
}
void MessageFiller::fillReplyInfo()
{
    ReplyInfo repInfo;
    repInfo._channelID  = testChannelID;
    repInfo._message    = testMessage;
    repInfo._msgID      = testUserID;
    repInfo._senderID   = testSenderID;
    repInfo._msgIdOwner = testMsgIDOwner;
    repInfo._userLogin  = testLogin;

    setReplyInfo(repInfo);
}
void MessageFiller::fillMessageInfo()
{
    MessageInfo mesgInfo;
    mesgInfo._channelID   = testChannelID;
    mesgInfo._message     = testMessage;
    mesgInfo._msgID       = testMsgID;
    mesgInfo._recipientID = testRecipientID;
    mesgInfo._senderID    = testSenderID;
    mesgInfo._userLogin   = testLogin;

    setMessageInfo(mesgInfo);
}
void MessageFiller::fillAllMessages() 
{
    fillLoginInfo();
    fillRegistrationInfo();
    fillChannelInfo();
    fillChannelLeaveInfo();
    fillChannelDeleteInfo();
    fillChannelSubscriptionInfo();
    fillReplyInfo();
    fillMessageInfo();
}
}  // namespace TestUtility
