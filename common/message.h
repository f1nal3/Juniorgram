#pragma once

#include "common.h"
#include "connection.h"

namespace network {
class Connection;

struct Message {
    enum class MessageType : uint32_t { ServerAccept, ServerPing, MessageAll, ServerMessage };

    struct MessageHeader {
        MessageType mID    = MessageType();
        uint32_t mBodySize = uint32_t();
    };

    std::shared_ptr<Connection> mRemote = nullptr;

    MessageHeader mHeader;
    std::vector<uint8_t> mBody;

    friend std::ostream& operator<<(std::ostream& os, const Message& message) {
        os << "ID:" << size_t(message.mHeader.mID) << " Size:" << message.mHeader.mBodySize;
        return os;
    }

    template <typename T>
    friend Message& operator<<(Message& message, const T& data) {
        static_assert(std::is_standard_layout<T>::value,
                      "Data is too complex to be pushed into vector");

        size_t i = message.mBody.size();
        message.mBody.resize(message.mBody.size() + sizeof(T));
        std::memcpy(message.mBody.data() + i, &data, sizeof(T));
        message.mHeader.mBodySize = message.mBody.size();
        return message;
    }

    template <typename T>
    friend Message& operator>>(Message& message, T& data) {
        static_assert(std::is_standard_layout<T>::value,
                      "Data is too complex to be pulled from vector");

        size_t i = message.mBody.size() - sizeof(T);
        std::memcpy(&data, message.mBody.data() + i, sizeof(T));
        message.mBody.resize(i);
        message.mHeader.mBodySize = message.mBody.size();
        return message;
    }
};
}  // namespace network
