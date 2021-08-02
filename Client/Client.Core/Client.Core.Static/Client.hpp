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

    void askForMessageHistory(const std::uint64_t channellID) const;

    void storeMessage(const std::string& message, const uint64_t userID, const uint64_t channelID) const;

    void userRegistration(const std::string& email, const std::string& login,
                          const std::string& password) const;
    void userAuthorization(const std::string& login, const std::string& password);

    void messageAll() const;

    void messageUserDelete(const uint64_t userId, const uint64_t messageId) const;
};
}  // namespace Network
