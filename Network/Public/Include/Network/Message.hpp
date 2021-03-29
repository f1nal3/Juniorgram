#pragma once

#include "Connection.hpp"
#include "Utility/Utility.hpp"
#include "Utility/WarningSuppression.hpp"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <memory>
#include <cstring>
#include <vector>

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
        ChannelListRequest
    };

    struct MessageHeader
    {
        MessageType mID         = MessageType();
        std::uint32_t mBodySize = std::uint32_t();
        std::chrono::time_point<std::chrono::system_clock> mTimestamp = std::chrono::system_clock::now();
    };

    std::shared_ptr<Connection> mRemote = nullptr;

    MessageHeader mHeader;
    std::vector<uint8_t> mBody;

    friend std::ostream& operator<<(std::ostream& os, const Message& message)
    {
        std::tm formattedTimestamp =
            utility::safe_localtime(std::chrono::system_clock::to_time_t(message.mHeader.mTimestamp));

        os << "ID:" << size_t(message.mHeader.mID) << " Size:" << message.mHeader.mBodySize
           << "Timestamp:" << std::put_time(&formattedTimestamp, "%F %T");
        return os;
    }

    template <typename T>
    friend Message& operator<<(Message& message, const T& data)
    {
        static_assert(std::is_standard_layout<T>::value,
                      "Data is too complex to be pushed into vector");

        size_t i = message.mBody.size();
        message.mBody.resize(message.mBody.size() + sizeof(T));
        std::memcpy(message.mBody.data() + i, &data, sizeof(T));
        message.mHeader.mBodySize = static_cast<std::uint32_t>(message.mBody.size());
        return message;
    }

    template <typename T>
    friend Message& operator>>(Message& message, T& data)
    {
        static_assert(std::is_standard_layout<T>::value,
                      "Data is too complex to be pulled from vector");

        size_t i = message.mBody.size() - sizeof(T);
        std::memcpy(&data, message.mBody.data() + i, sizeof(T));
        message.mBody.resize(i);
        message.mHeader.mBodySize = static_cast<std::uint32_t>(message.mBody.size());
        return message;
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
}  // namespace Network
