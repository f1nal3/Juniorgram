#pragma once

#include <any>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <memory>
#include <vector>

#include "Connection.hpp"
#include "Utility/Utility.hpp"

namespace Network
{
class Connection;

struct Message
{
    enum class MessageType : std::uint32_t
    {
        ServerAccept,
        ServerPing,
        MessageAll,
        ServerMessage,
        ChannelListRequest,
        MessageHistoryRequest,
        MessageStoreRequest,
        RegistrationRequest,
        RegistrationRequestToClient
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

    friend std::ostream& operator<<(std::ostream& os, const Message& message)
    {
        std::tm formattedTimestamp = Utility::safe_localtime(
            std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

        os << "ID:" << size_t(message.mHeader.mMessageType)
           << " Size:" << message.mHeader.mBodySize
           << "Timestamp:" << std::put_time(&formattedTimestamp, "%F %T");
        return os;
    }

    friend bool operator<(const Message& lhs, const Message& rhs)
    {
        return lhs.mHeader.mTimestamp < rhs.mHeader.mTimestamp;
    }

    friend bool operator>(const Message& lhs, const Message& rhs)
    {
        return lhs.mHeader.mTimestamp > rhs.mHeader.mTimestamp;
    }
};

template <typename Archive>
void serialize(Archive& ar, Message::MessageHeader& o)
{
    ar& o.mMessageType& o.mBodySize& o.mTimestamp;
}
}  // namespace Network
