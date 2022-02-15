#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include <FileLogger.hpp>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

namespace DataAccess
{
class PostgreRepositoryManager;
}

namespace Server
{
/**
 *  @class Server
 *  @brief Server class.
 */
class Server
{
private:
    uint64_t mIDCounter         = 10000;
    uint64_t mCriticalQueueSize = 100;
    uint64_t mNewThreadsCount   = std::thread::hardware_concurrency();

    asio::io_context                                       mContext;
    asio::ip::tcp::acceptor                                mAcceptor;
    std::deque<std::shared_ptr<Network::Connection>>       mConnectionsPointers;
    Network::SafeQueue<Network::Message>                   mIncomingMessagesQueue;
    std::deque<std::thread>                                mThreads;
    std::unique_ptr<DataAccess::PostgreRepositoryManager>  mPostgreManager;

private:
    /**
     * @brief Method for sending the message connecting to the server.
     * @param Connection management class as std::shared_ptr<Network::Connection>&.
     */
    static bool onClientConnect(const std::shared_ptr<Network::Connection>& client);

    /**
     * @brief Method for sending the message disconnecting from the server.
     * @param Connection management class as std::shared_ptr<Network::Connection>&.
     */
    static void onClientDisconnect(const std::shared_ptr<Network::Connection>& client);

    /**
     * @brief Method used to process messages.
     * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     */
    void onMessage(const std::shared_ptr<Network::Connection>& client, Network::Message& message);

public:
    explicit Server(const uint16_t& port);

    ~Server();

    /**
     * @brief Method to start the server.
     */
    void start();

    //void registerRepositories();

    /**
     * @brief Method to stop the server.
     */
    void stop();

    /**
     * @brief Method for connecting to a client, works asynchronously.
     */
    void waitForClientConnection();

    /**
     * @brief Method for sending the message from client.
     * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     */
    void messageClient(std::shared_ptr<Network::Connection> client, const Network::Message& message);

    /**
     * @brief Method of sending a message to all clients.
     * @param Network::Message& class and connection management class as std::shared_ptr<Network::Connection>&.
     */
    void messageAllClients(const Network::Message& message, const std::shared_ptr<Network::Connection>& exceptionClient = nullptr);

    /**
     * @brief Method for updating messages.
     * @param std::size_t limit and bool for method wait() in SafeQueue.
     */
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);
};
}  // namespace Server
