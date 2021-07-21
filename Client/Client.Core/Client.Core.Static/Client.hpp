#pragma once

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

namespace Network
{
class Client
{
    asio::io_context mContext;
    std::thread mContextThread;

    std::unique_ptr<Connection> mConnection;

    SafeQueue<Message> mIncomingMessagesQueue;

public:
    ~Client();

    bool connect(const std::string_view& host, const uint16_t port);

    void disconnect();

    bool isConnected() const;

    void send(const Message& message) const;

    SafeQueue<Message>& incoming() { return mIncomingMessagesQueue; }

    void pingServer() const;

    void askForChannelList() const;

    void askForMessageHistory() const;

    void storeMessages(const std::vector<std::string>& messagesList) const;

    void userRegistration(const std::string& email, const std::string& login,
                          const std::string& password) const;

    void messageAll() const;

    void messageUserDelete(uint64_t userId, uint64_t messageId) const;
};
}  // namespace Network
