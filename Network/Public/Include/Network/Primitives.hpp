#pragma once

#include <Utility/Utility.hpp>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <string>

namespace Network
{
/**
 * @brief The ChannelInfo struct
 * @details Channel Info struct for information about channels
 *          contain creator ID, channel ID and channel Name.
 */
struct ChannelInfo
{
    /// creator ID uint64_t variable
    std::uint64_t creatorID;

    /// channel ID uint64_t variable
    std::uint64_t channelID;

    /// channel name string variable
    std::string   channelName;

public:
    /// Default ChannelInfo constructor
    ChannelInfo() = default;

    /// ChannelInfo constructor with initializing list
    ChannelInfo(const std::uint64_t creatorID, const std::uint64_t channelID, const std::string& channelName)
        : creatorID(creatorID), channelID(channelID), channelName(channelName)
    {
    }

    /// Default ChannelInfo destructor
    ~ChannelInfo() = default;

    /// Operator == for comparisons channels info
    friend bool operator==(const ChannelInfo& first, const ChannelInfo& second)
    {
        return first.channelID == second.channelID && first.creatorID == second.creatorID && first.channelName == second.channelName;
    }
};

/// Serialize method which serialize each field
template <typename Archive>
void serialize(Archive& ar, Network::ChannelInfo& o)
{
    ar& o.creatorID& o.channelID& o.channelName;
}

/**
 * @brief Struct for storing and transmitting login information
 *  @field login users login as std::string
 *  @field pwdHash hash of user's password
 */
struct LoginInfo
{
    /// login string variable
    std::string login;

    /// pwdHash string variable
    std::string pwdHash;

    /// Default LoginInfo constructor
    LoginInfo()                 = default;

    /// Default LoginInfo copy constructor
    LoginInfo(const LoginInfo&) = default;

    /// LoginInfo constructor with initializing list
    explicit LoginInfo(const std::string& login, const std::string& passwordHash) : login(login), pwdHash(passwordHash) {}

    /// Default LoginInfo destructor
    ~LoginInfo() = default;
};

/**
 * @brief helper function for serializing @ref LoginInfo structure
 */
template <typename Archive>
void serialize(Archive& ar, LoginInfo& o)
{
    ar& o.login& o.pwdHash;
}

/**
 * @brief The RegistrationInfo struct
 * @details Info about email, login and passwordHash.
 */
struct RegistrationInfo
{
    /// email string variable
    std::string email;

    /// login string variable
    std::string login;

    /// passwordHash string variable
    std::string passwordHash;

    /// Default RegistrationInfo constructor
    RegistrationInfo() = default;

    /// RegistrationInfo constructor with initializing list
    explicit RegistrationInfo(const std::string& email, const std::string& login, const std::string& passwordHash)
        : email(email), login(login), passwordHash(passwordHash)
    {
    }

    /// Default RegistrationInfo copy constructor
    RegistrationInfo(const RegistrationInfo&) = default;

    /// Default RegistrationInfo destructor
    ~RegistrationInfo()                       = default;

    /// Default Registration copy assignment constructor
    RegistrationInfo& operator=(const RegistrationInfo& other) = default;

    /// Operator == for comparison registration info
    friend bool operator==(const RegistrationInfo& registrationInfo1, const RegistrationInfo& registrationInfo2)
    {
        return registrationInfo1.email == registrationInfo2.email && registrationInfo1.login == registrationInfo2.login &&
               registrationInfo1.passwordHash == registrationInfo2.passwordHash;
    }
};

/// Serialize method for serialize registration info for each field
template <typename Archive>
void serialize(Archive& ar, Network::RegistrationInfo& o)
{
    ar& o.email& o.login& o.passwordHash;
}

/**
 * @brief The MessageDeletedInfo struct
 * @details Deleted Information contains user ID and message ID
 */
struct MessageDeletedInfo
{
    /// userId uint64_t variable
    uint64_t userId;

    /// messageId uint64_t variable
    uint64_t messageId;

    /// MessageDeletedInfo with initializing list
    explicit MessageDeletedInfo(const uint64_t _userId, const uint64_t _messageId) : userId(_userId), messageId(_messageId) {}

    /// Operator == for comparison message deleted info
    friend bool operator==(const MessageDeletedInfo& messageDeletedInfo1, const MessageDeletedInfo& messageDeletedInfo2)
    {
        return messageDeletedInfo1.messageId == messageDeletedInfo2.messageId && messageDeletedInfo1.userId == messageDeletedInfo2.userId;
    }
};

/// Serialize method for serialize deleted info for each field
template <typename Archive>
void serialize(Archive& ar, Network::MessageDeletedInfo& o)
{
    ar& o.userId& o.messageId;
}

/**
 * @brief The MessageInfo struct
 * @details Message Info contains channel ID, message, message ID,
 *          sender ID, recipient ID and time.
 */
struct MessageInfo
{
    /// channel ID uint64_t variable
    std::uint64_t channelID;

    /// message string variable
    std::string   message;

    /// msg ID uint64_t variable
    std::uint64_t msgID;

    /// sender ID uint64_t variable
    std::uint64_t senderID;

    /// recipient ID uint64_t variable
    std::uint64_t recipientID;

    /// time string variable
    std::string   time = Utility::getTimeNow();

    /// Default MessageIndo constructor
    MessageInfo() = default;

    /// MessageIndo constructor with initializing list
    explicit MessageInfo(const uint64_t channelID, const std::string& text) : channelID(channelID), message(text) {}

    /// Default MessageIndo copy constructor
    MessageInfo(const MessageInfo&) = default;

    /// Default MessageIndo destructor
    ~MessageInfo()                  = default;

    /// Operator == for comparison Message Info
    friend bool operator==(const MessageInfo& first, const MessageInfo& second)
    {
        return first.message == second.message && first.channelID == second.channelID && first.time == second.time;
    }
};

/// Serialize method for serialize Message Info for each field
template <typename Archive>
void serialize(Archive& ar, Network::MessageInfo& o)
{
    ar& o.channelID& o.message& o.time;
}

}  // namespace Network
