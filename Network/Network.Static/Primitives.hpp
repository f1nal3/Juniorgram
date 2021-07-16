#pragma once

#include <Utility/WarningSuppression.hpp>
#include <cstdint>
#include <string>
#include <chrono>
#include <cstring>
        
namespace Network
{
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

    template <typename Archive>
    void serialize(Archive& ar, Network::ChannelInfo& o);
    
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

    template <typename Archive>
    void serialize(Archive& ar, Network::MessageInfo& o);

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
    void serialize(Archive& ar, Network::RegistrationInfo& o);
} // namespace Network
