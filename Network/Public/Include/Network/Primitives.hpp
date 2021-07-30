#pragma once

//#include <Utility/WarningSuppression.hpp>
#include <cstdint>
#include <string>
#include <chrono>
#include <cstring>
#include <string>

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

        friend bool operator==(const ChannelInfo& channelInfo1, const ChannelInfo& channelInfo2)
        {
            return (channelInfo1.channelID == channelInfo2.channelID &&
                    channelInfo1.channelName == channelInfo2.channelName);
        }
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::ChannelInfo& o)
    {
        ar& o.creatorID& o.channelID& o.channelName;
    }
    
    struct MessageInfo
    {
        std::uint64_t userID;
        std::string message;

    public:
        MessageInfo() = default;
        MessageInfo(std::uint64_t userID, std::string message) : userID(userID), message(message) {}
        virtual ~MessageInfo() = default;

        friend bool operator==(const MessageInfo& messageInfo1, const MessageInfo& messageInfo2)
        {
            return (messageInfo1.userID == messageInfo2.userID &&
                    messageInfo1.message == messageInfo2.message);
        }
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageInfo& o)
    {
        ar& o.userID& o.message;
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

    struct MessageDeletedInfo
    {
        uint64_t userId;
        uint64_t messageId;
        explicit MessageDeletedInfo(const uint64_t _userId, const uint64_t _messageId)
            : userId(_userId), messageId(_messageId)
        {}

        friend bool operator==(const MessageDeletedInfo& messageDeletedInfo1,
            const MessageDeletedInfo& messageDeletedInfo2)
        {
            return messageDeletedInfo1.messageId == messageDeletedInfo2.messageId &&
                   messageDeletedInfo1.userId    == messageDeletedInfo2.userId;
        }
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageDeletedInfo& o)
    {
        ar& o.userId& o.messageId;
    }

    struct MessageStoringInfo : MessageInfo
    {
        std::uint64_t channelID;
        
        MessageStoringInfo() = default;
        explicit MessageStoringInfo(const uint64_t userID, const uint64_t channelID, const std::string& text) 
            : MessageInfo(userID, text), channelID(channelID) 
        {}

        MessageStoringInfo(const MessageStoringInfo&) = default;
        ~MessageStoringInfo()                         = default;

        friend bool operator==(const MessageStoringInfo& first, const MessageStoringInfo& second)
        {
            return first.message   == second.message && 
                   first.userID    == second.userID  && 
                   first.channelID == second.channelID;
        }
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageStoringInfo& o)
    {
        ar& o.channelID& o.message& o.userID;
    }

} // namespace Network
