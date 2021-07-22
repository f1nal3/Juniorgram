#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <cstring>

#include <Utility/WarningSuppression.hpp>
  
namespace Network
{
    struct ClientPayload
    {
        ClientPayload(const std::string& login, const std::string& psswdHash);

        ClientPayload& operator=(const ClientPayload& other) = default;

        friend bool operator==(const ClientPayload& payload, const ClientPayload& payloadAnother);

    private:
        std::string mOS;
        std::string mSub;
        std::string mFingerprint;
        std::string mIP;
    };

    struct ChannelInfo
    {
        std::uint64_t channelID;
        std::string channelName;

    public:
        ChannelInfo() = default;
        ChannelInfo(std::uint64_t channelID, std::string channelName)
            : channelID(channelID), channelName(channelName) {}
        ~ChannelInfo() = default;

        friend bool operator==(const ChannelInfo& channelInfo1, const ChannelInfo& channelInfo2);
    };

    
    struct MessageInfo
    {
        std::uint64_t userID;
        std::string message;

    public:
        MessageInfo() = default;
        MessageInfo(std::uint64_t userID, std::string message) : userID(userID), message(message) {}
        ~MessageInfo() = default;

        friend bool operator==(const MessageInfo& messageInfo1, const MessageInfo& messageInfo2);
    };


    struct RegistrationInfo
    {
        std::string email;
        std::string login;
        std::string passwordHash;

    public:
        RegistrationInfo() = default;
        explicit RegistrationInfo(const std::string& email, const std::string& login,
                                  const std::string& passwordHash);

        RegistrationInfo(const RegistrationInfo&) = default;
        ~RegistrationInfo()                       = default;


        RegistrationInfo& operator=(const RegistrationInfo& other) = default;

        friend bool operator==(const RegistrationInfo& registrationInfo1,
                               const RegistrationInfo& registrationInfo2);
    };

    template <typename Archive>
    void serialize(Archive& ar, Network::ChannelInfo& o)
    {
        ar& o.channelID& o.channelName;
    }

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageInfo& o)
    {
        ar& o.userID& o.message;
    }

    template <typename Archive>
    void serialize(Archive& ar, Network::RegistrationInfo& o)
    {
        ar& o.email& o.login& o.passwordHash;
    }
} // namespace Network
