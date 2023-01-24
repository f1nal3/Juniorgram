#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/iAPI.hpp"
#include "PostgreRepositoryManager.hpp"

#include "Utility/SafeQueue.hpp"

namespace DataAccess
{
class PostgreRepositoryManager;
}

namespace Server
{
using asio::ip::tcp;
using Network::Connection;
using Network::Message;
using Utility::SafeQueue;
using RepoManagerPtr = std::unique_ptr<DataAccess::IRepositoryManager>;

/*
* @brief Declaration of ServerBuilder.
*/
namespace Builder
{
class ServerBuilder;
}

/**
 *  @class Server
 *  @brief This class does all logic which is needed to run the server.
 *  @details This object is directly the root of the back-end. The main task
 *           is to communicate with clients, accepting messages from them, processing
 *           them according to certain logic prescribed in the handler functions (there is a
 *           handler for each type of message), and returning the result back to the client.
 *           See the API documentation in a file "Network/iAPI.h"
 *           The Server object is created through a special helper class, see ServerBuilder
 */
class Server : public Network::iAPI
{
    friend Builder::ServerBuilder;

public:
    /**
     * @brief Destructor
     */
    ~Server();

    /**
     * @brief Method to start the server.
     */
    void start();

    // void registerRepositories();

    /**
     * @brief Method to stop the server.
     */
    void stop();

    /**
     * @brief Method for updating messages.
     * @param std::size_t limit and bool for method wait() in SafeQueue.
     */
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);

private:
    /**
     * @brief Method for connecting to a client, works asynchronously.
     */
    void waitForClientConnection();

    /**
     * @brief Method for accepting to a client, works asynchronously.
     */
    void acceptingClientConnection(const std::error_code& error, asio::ip::tcp::socket& socket);

    /**
     * @brief Default constructor.
     * @details Initialize Server object. After you steel need to initialize network and database connection.
     */
    Server() = default;
    
    /**
     * @brief Setter for purpose of initialize IRepository dependency
     *
     * @param repoManager - pointer to instance of dependency repository
     */
    void initRepository(RepoManagerPtr repoManager);

    /**
     * @brief Setter for purpose of initialize host port. This is how the endpoint is configured.
     *
     * @param port - digital representation of port
     */
    void initConnection(const uint16_t port);

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
     * @param client management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     * @param message body of message
     */
    void onMessage(const std::shared_ptr<Connection>& client, const Message& message);

    /**
     *  This macros apply all method from api and avoid you from routine of handwriting
     */
    APPLY_API_METHODS

private:
    uint64_t _idCounter         = 10000;
    uint64_t _criticalQueueSize = 100;
    uint64_t _newThreadsCount   = std::thread::hardware_concurrency();

    asio::io_context                        _context;
    std::unique_ptr<tcp::acceptor>          _acceptor;
    std::deque<std::shared_ptr<Connection>> _connectionsPointers;
    SafeQueue<Message>                      _incomingMessagesQueue;
    std::deque<std::thread>                 _threads;
    RepoManagerPtr                          _repoManager;
};
}  // namespace Server
