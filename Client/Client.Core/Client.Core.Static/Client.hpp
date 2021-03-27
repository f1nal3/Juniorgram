#pragma once

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

namespace network
{
class Client
{
    asio::io_context mContext;
    std::thread mContextThread;

    std::unique_ptr<Connection> mConnection;

    SafeQueue<Message> mIncomingMessagesQueue;

public:
    ~Client();

    bool connect(const std::string& host, const uint16_t& port);

    void disconnect();

    bool isConnected() const;

    void send(const Message& message) const;

    SafeQueue<Message>& incoming() { return mIncomingMessagesQueue; }

    void pingServer() const;

    void askForChannelList() const;

    void messageAll() const;
};
}  // namespace network
