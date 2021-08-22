#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <cstring>
#include <string>
#include <Utility/Utility.hpp>

namespace Network
{
    struct ChannelInfo
    {
        std::uint64_t creatorID;
        std::uint64_t channelID;
        std::string channelName;

    public:
        ChannelInfo() = default;
        ChannelInfo(const std::uint64_t creatorID, const std::uint64_t channelID, const std::string& channelName)
            : creatorID(creatorID), channelID(channelID), channelName(channelName) {}
        ~ChannelInfo() = default;

        friend bool operator==(const ChannelInfo& first, const ChannelInfo& second)
        {
            return first.channelID   == second.channelID &&
                   first.creatorID   == second.creatorID &&
                   first.channelName == second.channelName;
        }
    };

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
        std::string login;
        std::string pwdHash;
        LoginInfo() = default;
        LoginInfo(const LoginInfo&) = default;
        explicit LoginInfo(const std::string& login, const std::string& passwordHash) :
                            login(login), pwdHash(passwordHash) {}
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

    struct RegistrationInfo
    {
        std::string email;
        std::string login;
        std::string passwordHash;

    public:
        RegistrationInfo() = default;
        explicit RegistrationInfo(const std::string& email, 
                                  const std::string& login,
                                  const std::string& passwordHash)
            : email(email), login(login), passwordHash(passwordHash)
        { }

        RegistrationInfo(const RegistrationInfo&) = default;
        ~RegistrationInfo()                       = default;


        RegistrationInfo& operator=(const RegistrationInfo& other) = default;

        friend bool operator==(const RegistrationInfo& registrationInfo1,
                               const RegistrationInfo& registrationInfo2)
        {
            return registrationInfo1.email    == registrationInfo2.email &&
                   registrationInfo1.login    == registrationInfo2.login &&
                   registrationInfo1.passwordHash == registrationInfo2.passwordHash;
        }
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::RegistrationInfo& o)
    {
        ar& o.email& o.login& o.passwordHash;
    }

    struct MessageInfo
    {
        std::uint64_t channelID;
        std::string message;
        std::uint64_t msgID;
        std::uint64_t senderID;
        std::uint64_t recipientID;
        std::string time = Utility::getTimeNow();

        MessageInfo() = default;
        explicit MessageInfo(const uint64_t channelID, const std::string& text)
            : channelID(channelID), message(text)
        {}

        MessageInfo(const MessageInfo&) = default;
        ~MessageInfo()                  = default;

        friend bool operator==(const MessageInfo& first, const MessageInfo& second)
        {
            return first.message   == second.message   &&
                   first.channelID == second.channelID &&
                   first.time      == second.time      &&
                   first.msgID     == second.msgID;
        }
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageInfo& o)
    {
        ar& o.channelID& o.message& o.msgID& o.time;
    }

} // namespace Network
