#pragma once

#include <any>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <memory>
#include <vector>

#include "Utility/Utility.hpp"

namespace Network
{
class Connection;

struct Message
{  
    enum class MessageType : std::uint64_t
    {
        ChannelListRequest          = 1 << 0,
        SetEncryptedConnection      = 1 << 1,
        ServerAccept                = 1 << 2,
        ServerPing                  = 1 << 3,
        ServerMessage               = 1 << 4,
        MessageAll                  = 1 << 5,
        MessageHistoryRequest       = 1 << 6,
        MessageStoreRequest         = 1 << 7,
        RevokeSession               = 1 << 8,   
        RegistrationRequest         = 1 << 9,
        RegistrationAnswer          = 1 << 10,
        RegistrationRequestToClient = 1 << 11,
        SendIV                      = 1 << 12,
        TokenRequest                = 1 << 13
    };
  

    struct MessageHeader
    {
        MessageType mMessageType = MessageType();
        std::uint32_t mBodySize   = std::uint32_t();
        std::uint32_t mOriginalSize = std::uint32_t();
        std::chrono::time_point<std::chrono::system_clock> mTimestamp =
            std::chrono::system_clock::now();
    };

    std::shared_ptr<Connection> mRemote = nullptr;

    MessageHeader mHeader;
    std::any mBody;

    friend std::ostream& operator<<(std::ostream& os, const Message& message);

    friend bool operator<(const Message& lhs, const Message& rhs);
 
    friend bool operator>(const Message& lhs, const Message& rhs);
};

template <typename Archive>
void serialize(Archive& ar, Message::MessageHeader& o);
}  // namespace Network
