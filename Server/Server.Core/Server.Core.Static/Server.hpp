#pragma once

#include "Network/Connection.hpp"
#include "Network/SafeQueue.hpp"
#include "Network/Message.hpp"

#include <asio.hpp>

#include <chrono>
#include <deque>
#include <iostream>
#include <thread>

namespace network
{
class Server
{
    uint64_t mIDCounter = 10000, mCriticalQueueSize = 100,
             mNewThreadsCount = std::thread::hardware_concurrency();

    asio::io_context mContext;

    asio::ip::tcp::acceptor mAcceptor;

    std::deque<std::shared_ptr<Connection>> mConnectionsPointers;

    SafeQueue<Message> mIncomingMessagesQueue;

    std::deque<std::thread> mThreads;

    bool onClientConnect(const std::shared_ptr<Connection>& client);

    void onClientDisconnect(const std::shared_ptr<Connection>& client);

    void onMessage(const std::shared_ptr<Connection>& client, Message& message);

public:
    explicit Server(const uint16_t& port);

    ~Server();

    bool start();

    void stop();

    void waitForClientConnection();

    void messageClient(std::shared_ptr<Connection> client, const Message& message);

    void messageAllClients(const Message& message,
                           const std::shared_ptr<Connection>& exceptionClient = nullptr);
    
    void update(size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);
};
}  // namespace network
