#include "Primitives.hpp"

namespace Network
{
    RegistrationInfo::RegistrationInfo(const std::string& email, const std::string& login,
                              const std::string& passwordHash)
        : email(email), login(login), passwordHash(passwordHash)
    {
    }

    bool operator==(const ChannelInfo& channelInfo1, const ChannelInfo& channelInfo2)
    {
        return (channelInfo1.channelID == channelInfo2.channelID &&
                channelInfo1.channelName == channelInfo2.channelName);
    }

    bool operator==(const MessageInfo& messageInfo1, const MessageInfo& messageInfo2)
    {
        return (messageInfo1.userID == messageInfo2.userID &&
                messageInfo1.message == messageInfo2.message);
    }

    bool operator==(const RegistrationInfo& registrationInfo1,
                    const RegistrationInfo& registrationInfo2)
    {
        return registrationInfo1.email == registrationInfo2.email &&
               registrationInfo1.login == registrationInfo2.login &&
               registrationInfo1.passwordHash == registrationInfo2.passwordHash;
    }

}  // namespace Network
