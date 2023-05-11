#pragma once

#include <chrono>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <cryptopp/rsa.h>

#include <Utility/Utility.hpp>
#include <Utility/UtilityTime.hpp>

namespace Models
{
const std::vector<std::string> reactionNames = {"likes", "dislikes", "fires", "cats", "smiles"};

/**
 * @brief The ChannelInfo struct
 * @details Channel Info struct for information about channels
 *          contain creator ID, channel ID and channel Name.
 */
struct ChannelInfo
{
    /// creator ID uint64_t variable
    std::uint64_t _creatorID;
    /// channel ID uint64_t variable
    std::uint64_t _channelID;
    /// channel name string variable
    std::string _channelName;

    /// Default ChannelInfo constructor
    ChannelInfo() = default;

    /// ChannelInfo constructor with initializing list
    ChannelInfo(const std::uint64_t creatorID_, const std::uint64_t channelID_, const std::string& channelName_)
        : _creatorID(creatorID_), _channelID(channelID_), _channelName(channelName_)
    {
    }
    /// Default ChannelInfo destructor
    ~ChannelInfo() = default;

    /// Operator == to compare channels info
    friend bool operator==(const ChannelInfo& first, const ChannelInfo& second)
    {
        return first._channelID   == second._channelID &&
               first._creatorID   == second._creatorID &&
               first._channelName == second._channelName;
    }
};

/// Serialize method which serialize each field
template <typename Archive>
void serialize(Archive& ar, Models::ChannelInfo& o)
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
    /// channel ID uint64_t variable
    std::uint64_t _channelID;
    /// user ID uint64_t variable
    std::uint64_t _userID;

    /// Default ChannelInfo constructor
    ChannelSubscriptionInfo() = default;

    /// ChannelInfo constructor with initializing list
    explicit ChannelSubscriptionInfo(const std::uint64_t channelID_) : _channelID(channelID_) {}
    /// Default ChannelSubscriptionInfo destructor
    ~ChannelSubscriptionInfo() = default;

    /// Operator == to compare channel info
    friend bool operator==(const ChannelSubscriptionInfo& first, const ChannelSubscriptionInfo& second)
    {
        return first._channelID == second._channelID &&
               first._userID    == second._userID;
    }
};

/**
 * @brief helper function for serializing
 * @ref ChannelSubscriptionInfo structure
 */
template <typename Archive>
void serialize(Archive& ar, Models::ChannelSubscriptionInfo& o)
{
    ar& o._channelID;
}

/**
 * @brief Struct for storing and transmitting login information
 */
struct LoginInfo
{
    /// user's login as string variable
    std::string _login;
    /// hash to authentificate user
    std::string _verifyingHash;
    /// Default LoginInfo constructor
    LoginInfo()                 = default;
    /// Default LoginInfo copy constructor
    LoginInfo(const LoginInfo&) = default;
    /// LoginInfo constructor with initializing list
    explicit LoginInfo(const std::string& login_, const std::string& verifyingHash_) : _login(login_), _verifyingHash(verifyingHash_) {}
    /// Default LoginInfo destructor
    ~LoginInfo() = default;
};

/**
 * @brief helper function for serializing @ref LoginInfo structure
 */
template <typename Archive>
void serialize(Archive& ar, Models::LoginInfo& o)
{
    ar& o._login& o._verifyingHash;
}

/**
 * @brief The RegistrationInfo struct
 * @details Info about email, login and passwordHash.
 */
struct RegistrationInfo
{
    /// email string variable
    std::string _email;
    /// login string variable
    std::string _login;
    /// passwordHash string variable
    std::string _passwordHash;
    /// Default RegistrationInfo constructor
    RegistrationInfo() = default;

    /// RegistrationInfo constructor with initializing list
    explicit RegistrationInfo(const std::string& email_, const std::string& login_, const std::string& passwordHash_)
        : _email(email_), _login(login_), _passwordHash(passwordHash_)
    {
    }

    /// Default RegistrationInfo copy constructor
    RegistrationInfo(const RegistrationInfo&) = default;
    /// Default RegistrationInfo destructor
    ~RegistrationInfo()                       = default;

    /// Default Registration copy assignment constructor
    RegistrationInfo& operator=(const RegistrationInfo& other) = default;

    /// Operator == to compare registration info
    friend bool operator==(const RegistrationInfo& first, const RegistrationInfo& second)
    {
        return first._email        == second._email &&
               first._login        == second._login &&
               first._passwordHash == second._passwordHash;
    }
};

/// Serialize method for serialize registration info for each field
template <typename Archive>
void serialize(Archive& ar, Models::RegistrationInfo& o)
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
    /// channel ID uint64_t variable
    std::uint64_t _channelID;
    /// message string variable
    std::string _message;
    /// msg ID uint64_t variable
    std::uint64_t _msgID{0};
    /// sender ID uint64_t variable
    std::uint64_t _senderID;
    /// user Login string variable
    std::string _userLogin;
    /// recipient ID uint64_t variable
    std::uint64_t _recipientID{0};
    /// time in milliseconds since Epoch (1970-01-01 00:00:00 UTC)
    UtilityTime::timestamp_t _time = UtilityTime::millisecondsSinceEpoch();
    /// reactions (reaction_id, reaction_count)
    std::map<std::uint32_t, std::uint32_t> _reactions = {};

    /// Default MessageIndo constructor
    MessageInfo() = default;
    /// MessageIndo constructor with initializing list
    explicit MessageInfo(const uint64_t channelID_, const std::string& message_) : _channelID(channelID_), _message(message_) {}
    /// Default MessageIndo copy constructor
    MessageInfo(const MessageInfo&) = default;
    /// Default MessageIndo destructor
    ~MessageInfo()                  = default;

    /// Operator == to compare Message Info
    friend bool operator==(const MessageInfo& first, const MessageInfo& second)
    {
        return first._message   == second._message   &&
               first._channelID == second._channelID &&
               first._time      == second._time      &&
               first._msgID     == second._msgID     &&
               first._userLogin == second._userLogin;
    }

    /// Operator < to compare MessageInfo
    friend bool operator<(const MessageInfo& lhs, const MessageInfo& rhs) 
    {
        return lhs._time < rhs._time;
    }
};

/// Serialize method for serialize Message Info for each field
template <typename Archive>
void serialize(Archive& ar, Models::MessageInfo& o)
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
    /// channel ID uint64_t variable
    std::uint64_t _channelID;
    /// message string variable
    std::string _message;
    /// msg ID uint64_t variable
    std::uint64_t _msgID{0};
    /// msg ID owner uint64_t variable
    std::uint64_t _msgIdOwner{0};
    /// sender ID uint64_t variable
    std::uint64_t _senderID{0};
    /// user Login string variable
    std::string _userLogin;
    /// Default ReplyInfo constructor
    ReplyInfo() = default;

    /// ReplyInfo constructor with initializing list
    explicit ReplyInfo(const std::uint64_t& channelID_, const std::string& message_) : _channelID(channelID_), _message(message_) {}
    /// Default Reply copy constructor
    ReplyInfo(const ReplyInfo&) = default;
    /// Default ReplyInfo destructor
    ~ReplyInfo()                = default;

    /// Operator == to compare Reply Info
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

/// Serialize method for serialize ReplyInfo for each field
template <typename Archive>
void serialize(Archive& ar, Models::ReplyInfo& o)
{
    ar& o._channelID& o._message& o._msgID& o._senderID& o._msgIdOwner& o._userLogin;
}

/**
 * @brief The ConnectoinInfo struct
 * @details Contains information, which will be used to verify connection(and client too)
 */
struct ConnectionInfo
{
    /// connection ID uint64_t variable
    std::uint64_t _connectionID;
    /// public server key, used to encrypt verification hash
    std::string _publicServerKey;
    /// Default ConnectoinInfo constructor
    ConnectionInfo() = default;

    /// ConnectoinInfo constructor with initializing list
    explicit ConnectionInfo(const std::uint64_t& connectionID_, const std::string& _publicServerKey_)
        : _connectionID(connectionID_), _publicServerKey(_publicServerKey_)
    {
    }
    /// Default ConnectoinInfo copy constructor
    ConnectionInfo(const ConnectionInfo&) = default;
    /// Default ConnectoinInfo destructor
    ~ConnectionInfo() = default;
};

/// Serialize method for serialize ConnectoinInfo for each field
template <typename Archive>
void serialize(Archive& ar, Models::ConnectionInfo& o)
{
    ar& o._connectionID& o._publicServerKey;
}

struct RSAKeyPair
{
    CryptoPP::RSA::PublicKey  publicKey;
    CryptoPP::RSA::PrivateKey  privateKey;

    std::string getPublicKeyAsString()
    {
        std::string publicKeyStr;
        CryptoPP::StringSink  stringSink(publicKeyStr);
        publicKey.DEREncode(stringSink);
        return publicKeyStr;
    }

    CryptoPP::RSA::PublicKey getPublicKeyFromString(const std::string& publicServerKeyStr)
    {
        CryptoPP::StringSource   stringSource(publicServerKeyStr, true);
        CryptoPP::RSA::PublicKey publicServerKey;
        publicServerKey.BERDecode(stringSource);
        return publicServerKey;
    }
};
}  // namespace Models
