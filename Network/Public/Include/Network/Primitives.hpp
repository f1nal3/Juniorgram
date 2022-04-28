#pragma once

#include <chrono>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#include <Utility/Utility.hpp>

namespace Network
{
/**
 * @brief The ChannelInfo struct
 * @details Channel Info struct for information about channels \
 *          contain creator ID, channel ID and channel Name.
 */
struct ChannelInfo
{
    /// creator ID uint64_t variable.
    std::uint64_t _creatorID;
    /// channel ID uint64_t variable.
    std::uint64_t _channelID;
    /// channel name string variable.
    std::string   _channelName;

public:
    /// Default ChannelInfo constructor.
    ChannelInfo() = default;

    /// ChannelInfo constructor with initializing list.
    ChannelInfo(const std::uint64_t creatorID, const std::uint64_t channelID, const std::string& channelName)
        : _creatorID(creatorID), _channelID(channelID), _channelName(channelName)
    {
    }
    /// Default ChannelInfo destructor.
    ~ChannelInfo() = default;

    /// Operator == to compare channels info.
    friend bool operator==(const ChannelInfo& first, const ChannelInfo& second)
    {
        return first._channelID   == second._channelID && 
               first._creatorID   == second._creatorID &&
               first._channelName == second._channelName;
    }
};

/// Serialize method which serialize each field.
template <typename TArchive>
void serialize(TArchive& ar, Network::ChannelInfo& o)
{
    ar& o._creatorID& o._channelID& o._channelName;
}

using ChannelDeleteInfo = ChannelInfo;
using ChannelLeaveInfo  = ChannelInfo;

/**
 * @brief The ChannelSubscriptionInfo struct
 * @details Channel Info struct for information about the channel that the user subscribed.
 *          contain channel ID.
 */
struct ChannelSubscriptionInfo
{
    /// channel ID uint64_t variable.
    std::uint64_t _channelID;
    /// user ID uint64_t variable.
    std::uint64_t _userID;

public:
    /// Default ChannelInfo constructor.
    ChannelSubscriptionInfo() = default;

    /// ChannelInfo constructor with initializing list.
    ChannelSubscriptionInfo(const std::uint64_t channelID) : _channelID(channelID) {}
    /// Default ChannelSubscriptionInfo destructor.
    ~ChannelSubscriptionInfo() = default;

    /// Operator == to compare channel info.
    friend bool operator==(const ChannelSubscriptionInfo& first, const ChannelSubscriptionInfo& second)
    {
        return first._channelID == second._channelID &&
                  first._userID == second._userID;
    }
};

/**
 * @brief helper function for serializing.
 * @ref ChannelSubscriptionInfo structure.
 */
template <typename TArchive>
void serialize(TArchive& ar, Network::ChannelSubscriptionInfo& o)
{
    ar& o._channelID;
}

/**
 * @brief Struct for storing and transmitting login information.
 */
struct LoginInfo
{
    /// User's login as string variable.
    std::string _login;
    /// pwdHash hash of user's password as string variable.
    std::string _PWDHash;
    /// Default LoginInfo constructor.
    LoginInfo()                 = default;
    /// Default LoginInfo copy constructor.
    LoginInfo(const LoginInfo&) = default;
    /// LoginInfo constructor with initializing list.
    explicit LoginInfo(const std::string& login, const std::string& passwordHash) : _login(login), _PWDHash(passwordHash) {}
    /// Default LoginInfo destructor.
    ~LoginInfo() = default;
};

/**
 * @brief helper function for serializing @ref LoginInfo structure.
 */
template <typename Archive>
void serialize(Archive& ar, LoginInfo& o)
{
    ar& o._login& o._PWDHash;
}

/**
 * @brief The RegistrationInfo struct
 * @details Info about email, login and passwordHash.
 */
struct RegistrationInfo
{
    /// email string variable.
    std::string _email;
    /// login string variable.
    std::string _login;
    /// passwordHash string variable.
    std::string _passwordHash;
    /// Default RegistrationInfo constructor.
    RegistrationInfo() = default;

    /// RegistrationInfo constructor with initializing list.
    explicit RegistrationInfo(const std::string& email, const std::string& login, const std::string& passwordHash)
        : _email(email), _login(login), _passwordHash(passwordHash)
    {
    }

    /// Default RegistrationInfo copy constructor.
    RegistrationInfo(const RegistrationInfo&) = default;
    /// Default RegistrationInfo destructor.
    ~RegistrationInfo()                       = default;

    /// Default Registration copy assignment constructor.
    RegistrationInfo& operator=(const RegistrationInfo& other) = default;

    /// Operator == to compare registration info.
    friend bool operator==(const RegistrationInfo& registrationInfo1, const RegistrationInfo& registrationInfo2)
    {
        return registrationInfo1._email        == registrationInfo2._email && 
               registrationInfo1._login        == registrationInfo2._login &&
               registrationInfo1._passwordHash == registrationInfo2._passwordHash;
    }
};

/// Serialize method for serialize registration info for each field.
template <typename TArchive>
void serialize(TArchive& ar, Network::RegistrationInfo& o)
{
    ar& o._email& o._login& o._passwordHash;
}

/**
 * @brief The MessageInfo struct
 * @details Message Info contains channel ID, message, message ID,
 *          sender ID, recipient ID and time.
 */
struct MessageInfo
{
    /// channel ID uint64_t variable.
    std::uint64_t _channelID;
    /// message string variable.
    std::string   _message;
    /// msg ID uint64_t variable.
    std::uint64_t _msgID;
    /// sender ID uint64_t variable.
    std::uint64_t _senderID;
    /// user Login string variable.
    std::string   _userLogin;
    /// recipient ID uint64_t variable.
    std::uint64_t _recipientID;
    /// time in milliseconds since Epoch (1970-01-01 00:00:00 UTC).
    std::uint64_t _time = Utility::millisecondsSinceEpoch();
    /// reactions (reaction_id, reaction_count).
    std::map<std::uint32_t, std::uint32_t> _reactions = {};

    /// Default MessageIndo constructor.
    MessageInfo() = default;
    /// MessageIndo constructor with initializing list.
    explicit MessageInfo(const uint64_t channelID, const std::string& text) : _channelID(channelID), _message(text) {}
    /// Default MessageIndo copy constructor.
    MessageInfo(const MessageInfo&) = default;
    /// Default MessageIndo destructor.
    ~MessageInfo()                  = default;

    /// Operator == to compare Message Info.
    friend bool operator==(const MessageInfo& first, const MessageInfo& second)
    {
        return first._message   == second._message   && 
               first._channelID == second._channelID &&
               first._time      == second._time      &&
               first._msgID     == second._msgID     &&
               first._userLogin == second._userLogin;
    }

    /// Operator < to compare MessageInfo.
    friend bool operator<(const MessageInfo& lhs, const MessageInfo& rhs) 
    {
        return lhs._time < rhs._time;
    }
};

/// Serialize method for serialize Message Info for each field.
template <typename TArchive>
void serialize(TArchive& ar, Network::MessageInfo& o)
{
    ar& o._channelID& o._senderID& o._msgID& o._message& o._reactions& o._time& o._userLogin;
}

/**
 * @brief The ReplyInfo struct
 * @details Reply Info contains channel ID, message, message ID,
 *          sender ID, msgIdOwner and sender ID.
 */
struct ReplyInfo
{
    /// channel ID uint64_t variable.
    std::uint64_t _channelID;
    /// message string variable.
    std::string   _message;
    /// msg ID uint64_t variable.
    std::uint64_t _msgID;
    /// msg ID owner uint64_t variable.
    std::uint64_t _msgIdOwner;
    /// sender ID uint64_t variable.
    std::uint64_t _senderID;
    /// user Login string variable.
    std::string   _userLogin;
    /// Default ReplyInfo constructor.
    ReplyInfo() = default;

    /// ReplyInfo constructor with initializing list.
    explicit ReplyInfo(const std::uint64_t& channelID, const std::string& text) : _channelID(channelID), _message(text) {}
    /// Default Reply copy constructor.
    ReplyInfo(const ReplyInfo&) = default;
    /// Default ReplyInfo destructor.
    ~ReplyInfo()                = default;

    /// Operator == to compare Reply Info.
    friend bool operator==(const ReplyInfo& first, const ReplyInfo& second)
    {
        return first._message    == second._message    &&
               first._msgID      == second._msgID      &&
               first._senderID   == second._senderID   &&
               first._msgIdOwner == second._msgIdOwner &&
               first._channelID  == second._channelID  &&
               first._userLogin  == second._userLogin;
    }
};

/// Serialize method for serialize ReplyInfo for each field.
template <typename TArchive>
void serialize(TArchive& ar, Network::ReplyInfo& o)
{
    ar& o._channelID& o._message& o._msgID& o._senderID& o._msgIdOwner& o._userLogin;
}

}  /// namespace Network
