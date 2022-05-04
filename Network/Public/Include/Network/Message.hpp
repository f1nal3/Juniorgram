#pragma once

#include <any>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <memory>
#include <vector>
#include <Utility/Utility.hpp>

#include "Connection.hpp"

namespace Network
{
class Connection;

/**
 * @brief The Message struct
 */
struct Message
{
    /// Enum class for Message Type
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
        MessageType                                        mMessageType = MessageType();
        std::uint32_t                                      mBodySize    = std::uint32_t();
        std::uint64_t                                      mTimestamp   = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    };

    /// Connection variable
    std::shared_ptr<Connection> mRemote = nullptr;

    /// Message Header variable
    MessageHeader mHeader;
    /// Message Body variable
    std::any mBody;

    /// Stream for out message about message ID and Timestapm
    friend std::ostream& operator<<(std::ostream& os, const Message& message)
    {
        std::tm formattedTimestamp = UtilityTime::safe_localtime(message.mHeader.mTimestamp);

        os << "ID:" << size_t(message.mHeader.mMessageType) << " Size:" << message.mHeader.mBodySize
           << "Timestamp:" << std::put_time(&formattedTimestamp, "%F %T");
        return os;
    }

    /// Operator < for comparison timestamps
    friend bool operator<(const Message& lhs, const Message& rhs) { return lhs.mHeader.mTimestamp < rhs.mHeader.mTimestamp; }

    /// Operator > for comparison timestamps
    friend bool operator>(const Message& lhs, const Message& rhs) { return lhs.mHeader.mTimestamp > rhs.mHeader.mTimestamp; }
};

/// Serialize method for MessageHeader which serialize each field
template <typename Archive>
void serialize(Archive& ar, Message::MessageHeader& o)
{
    ar& o.mMessageType& o.mBodySize& o.mTimestamp;
}
}  // namespace Network
