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
        std::uint64_t channelID;
        std::string channelName;

    public:
        ChannelInfo() = default;
        ChannelInfo(std::uint64_t channelID, std::string channelName)
            : channelID(channelID), channelName(channelName) {}
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
        ar& o.channelID& o.channelName;
    }
    
    struct MessageInfo
    {
        std::uint64_t userID;
        std::string message;

    public:
        MessageInfo() = default;
        MessageInfo(std::uint64_t userID, std::string message) : userID(userID), message(message) {}
        ~MessageInfo() = default;

        friend bool operator==(const MessageInfo& messageInfo1, const MessageInfo& messageInfo2)
        {
            return (messageInfo1.userID == messageInfo2.userID &&
                    messageInfo1.message == messageInfo2.message);
        }
    };

    struct RegistrationInfo
    {
    private:
        std::string email;
        std::string login;
        std::string password;

    public:
        RegistrationInfo() = default;
        explicit RegistrationInfo(const std::string& email, 
                                  const std::string& login,
                                  const std::string& password)
            : email(email), login(login), password(password)
        { }
        ~RegistrationInfo() = default;

        friend bool operator==(const RegistrationInfo& registrationInfo1,
                               const RegistrationInfo& registrationInfo2)
        {
            return registrationInfo1.email    == registrationInfo2.email &&
                   registrationInfo1.login    == registrationInfo2.login &&
                   registrationInfo1.password == registrationInfo2.password;
        }
        
        inline std::string getEmail() const { return email; }
        inline std::string getLogin() const  { return login; }
        inline std::string getPassword() const { return password; }
    };
} // namespace Network
