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

/**
 * @brief The Message struct
 */
struct Message
{
    /// Enum class for Message Type.
    enum class MessageType : std::uint32_t
    {
        ServerAccept,
        ServerPing,
        MessageAll,
        ServerMessage,
        ChannelListRequest,
        MessageHistoryRequest,
        MessageHistoryAnswer,
        MessageStoreRequest,
        MessageStoreAnswer,
        RegistrationRequest,
        RegistrationAnswer,
        LoginRequest,
        LoginAnswer,
        MessageDeleteRequest,
        MessageDeleteAnswer,
        MessageEditRequest,
        MessageEditAnswer,
        ReplyHistoryRequest,
        ReplyHistoryAnswer,
        ReplyStoreRequest,
        ReplyStoreAnswer,
        ChannelLeaveRequest,
        ChannelLeaveAnswer,
        ChannelSubscribeRequest,
        ChannelSubscribeAnswer,
        ChannelSubscriptionListRequest,
        ChannelSubscriptionListAnswer,
        ChannelDeleteRequest,
        ChannelDeleteAnswer,
        ChannelCreateRequest,
        ChannelCreateAnswer,
        DirectMessageCreateRequest,
        DirectMessageCreateAnswer,
        MessageReactionRequest,
        MessageReactionAnswer
    };

    /**
     * @brief The MessageHeader struct
     * @details Contain Message Type class, body size and timestamp.
     */
    struct MessageHeader
    {
        MessageType                                        _messageType = MessageType();
        std::uint32_t                                      _bodySize    = std::uint32_t();
        std::chrono::time_point<std::chrono::system_clock> _timestamp   = std::chrono::system_clock::now();
    };

    /// Connection variable.
    std::shared_ptr<Connection> _remote = nullptr;

    /// Message Header variable.
    MessageHeader _header;
    /// Message Body variable.
    std::any _body;

    /// Stream for out message about message ID and Timestapm.
    friend std::ostream& operator<<(std::ostream& os, const Message& message)
    {
        std::tm formattedTimestamp = Utility::safe_localtime(std::chrono::system_clock::to_time_t(message._header._timestamp));

        os << "ID:" << size_t(message._header._messageType) << " Size:" << message._header._bodySize
           << "Timestamp:" << std::put_time(&formattedTimestamp, "%F %T");
        return os;
    }

    /// Operator < for comparison timestamps.
    friend bool operator<(const Message& lhs, const Message& rhs) { return lhs._header._timestamp < rhs._header._timestamp; }

    /// Operator > for comparison timestamps.
    friend bool operator>(const Message& lhs, const Message& rhs) { return lhs._header._timestamp > rhs._header._timestamp; }
};

/// Serialize method for MessageHeader which serialize each field.
template <typename TArchive>
void serialize(TArchive& ar, Message::MessageHeader& o)
{
    ar& o._messageType& o._bodySize& o._timestamp;
}
}  /// namespace Network
