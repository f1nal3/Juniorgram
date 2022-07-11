#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include <FileLogger.hpp>
#include "Network/iAPI.hpp"
#include "PostgreRepositoryManager.hpp"

namespace DataAccess
{
class PostgreRepositoryManager;
}

namespace Server
{
using asio::ip::tcp;
using Network::Message;
using Network::SafeQueue;
using Network::Connection;
using Base::Logger::LogLevel;
using Base::Logger::FileLogger;
using DataAccess::PostgreRepositoryManager;

/**
 * @brief 
 * 
 */
namespace Builder
{
    class ServerBuilder;
}

/**
 *  @class Server class
 *  @brief This class does all logic which is needed to run the server.
 *  @details Uses std::asio tools.
 */
class Server : public Network::iAPI
{
    friend Builder::ServerBuilder;

public:
     APPLY_API_METHODS;

public: 
    /**
    * @brief Destructor
    */
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
         * @brief Method for updating messages.
         * @param std::size_t limit and bool for method wait() in SafeQueue.
         */
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);

    /**
     * @brief Method for accepting to a client, works asynchronously.
     */
    void acceptingClientConnection(const std::error_code& error, asio::ip::tcp::socket& socket);

private:
    /**
    * @brief constructor.
    * @details Initialize Server object. After you steel need to initialize network and database connection.
    */
    Server();
    void initRepository(std::unique_ptr<PostgreRepositoryManager> postgreManager);
    void initConnection(const uint16_t port);

private:
    /**
     * @brief Method for sending the message connecting to the server.
     * @param Connection management class as std::shared_ptr<Network::Connection>&.
     */
    static bool onClientConnect(const std::shared_ptr<Connection>& client);

    /**
     * @brief Method for sending the message disconnecting from the server.
     * @param Connection management class as std::shared_ptr<Network::Connection>&.
     */
    static void onClientDisconnect(const std::shared_ptr<Connection>& client);

    /**
     * @brief Method used to process messages.
     * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     */
    void onMessage(const std::shared_ptr<Connection>& client, Message& message);

private:
    uint64_t _idCounter         = 10000;
    uint64_t _criticalQueueSize = 100;
    uint64_t _newThreadsCount   = std::thread::hardware_concurrency();

    asio::io_context                                       _context;
    std::unique_ptr <tcp::acceptor>                        _acceptor;
    std::deque<std::shared_ptr<Connection>>                _connectionsPointers;
    SafeQueue<Message>                                     _incomingMessagesQueue;
    std::deque<std::thread>                                _threads;
    std::unique_ptr<PostgreRepositoryManager>              _postgreManager;
};
}  // namespace Server
