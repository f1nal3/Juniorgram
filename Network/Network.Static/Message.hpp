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
    enum class MessageType : std::uint32_t
    {
        ChannelListRequest,
        SetEncryptedConnection,
        ServerAccept,
        ServerPing,
        ServerMessage,
        MessageAll,
        MessageHistoryRequest,
        MessageStoreRequest,
        RegistrationRequest,
        RegistrationAnswer,
        RegistrationRequestToClient,
        SendIV,
        TokenRequest
    };

    struct MessageHeader
    {
        MessageType mMessageType = MessageType();
        std::uint32_t mBodySize   = std::uint32_t();
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
