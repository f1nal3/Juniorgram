#include "Primitives.hpp"

#include <Utility.Static/Cryptography.hpp>
#include <Utility.Static/MACAddress.hpp>
      
namespace Network
{
    RegistrationInfo::RegistrationInfo(const std::string& email, const std::string& login,
                              const std::string& passwordHash)
        : email(email), login(login), passwordHash(passwordHash)
    {
    }

    ClientPayload::ClientPayload(const std::string& login,
                                 const std::string& psswdHash)
        : mSub{Hashing::getSHA1HashingValue(Utility::MACAddressUtility::GetMACAddress() + login +
                                            psswdHash)},
            mOS{[]() -> std::string {
            #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                            return "Windows";
            #elif __APPLE__
                            return "MacOS";
            #elif __linux__
                            return "Linux";
            #endif
                        }()},
            mTag{"jg.client"}
    {
    }

    bool operator==(const ClientPayload& payload, const ClientPayload& payloadAnother)
        {
            return payload.mOS == payloadAnother.mOS && payload.mSub == payloadAnother.mSub &&
                   payload.mTag == payloadAnother.mTag;
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
