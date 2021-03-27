#pragma once

#include "DataAccess/IRepository.hpp"
#include "Network/Connection.hpp"
#include "Network/SafeQueue.hpp"
#include "Network/Message.hpp"

#include <asio.hpp>

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

namespace server
{

class Server
{
    uint64_t mIDCounter = 10000, mCriticalQueueSize = 100,
             mNewThreadsCount = std::thread::hardware_concurrency();

    asio::io_context mContext;
    asio::ip::tcp::acceptor mAcceptor;
    std::deque<std::shared_ptr<network::Connection>> mConnectionsPointers;
    network::SafeQueue<network::Message> mIncomingMessagesQueue;
    std::deque<std::thread> mThreads;
    std::unique_ptr<DataAccess::IRepository> _postgreRepo;

    bool onClientConnect(const std::shared_ptr<network::Connection>& client);

    void onClientDisconnect(const std::shared_ptr<network::Connection>& client);

    void onMessage(const std::shared_ptr<network::Connection>& client, network::Message& message);

public:
    explicit Server(const uint16_t& port);

    ~Server();

    bool start();

    void stop();

    void waitForClientConnection();

    void messageClient(std::shared_ptr<network::Connection> client,
                       const network::Message& message);

    void messageAllClients(const network::Message& message,
                           const std::shared_ptr<network::Connection>& exceptionClient = nullptr);
    
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);
};
}  // namespace network
