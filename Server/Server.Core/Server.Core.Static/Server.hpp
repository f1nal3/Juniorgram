#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>
#include <optional>
#include <random>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/iAPI.hpp"
#include "PostgreRepositoryManager.hpp"
#include "RSAKeyManager.hpp"


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
using Network::MessageResult;
using RepoManagerUPtr = std::unique_ptr<DataAccess::IRepositoryManager>;
using RSAKeyManagerSPtr = std::shared_ptr<Base::RSAKeyManager>;
using UInt64Dist = std::uniform_int_distribution<uint64_t>;

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
*           See the API documentation in a file "Network/iAPI.h".
*           The Server object is created through a special helper class, see ServerBuilder.
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
    void initRepository(RepoManagerUPtr repoManager);

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
    void onMessage(const std::shared_ptr<Connection>& client, const Message& message) const;

    /**
     * @brief Method to check message result after that server answered to message.
     */
    void checkMessageResult(std::optional<MessageResult> result);


    /**
     * @brief Method to get a container with the results of message processing.
     * @details The results of the messages may be as follows:
     *          InvalidBody  - poorly formed message. /
     *          Success      - correctly formed message.
     */
    std::vector<MessageResult> getMessageResult() const;

    /**
     *  This macros apply all method from api and avoid you from routine of handwriting
     */
    APPLY_API_METHODS

private:
    uint64_t _criticalQueueSize = 100;
    uint64_t _newThreadsCount   = std::thread::hardware_concurrency();

    std::random_device _randDevice;
    std::mt19937       _randGenerator = std::mt19937(_randDevice());
    UInt64Dist         _distance      = UInt64Dist(10000, 100000);

    asio::io_context                        _context;
    std::vector<MessageResult>              _messageResponce;
    std::deque<std::shared_ptr<Connection>> _connectionsPointers;
    SafeQueue<Message>                      _incomingMessagesQueue;
    std::deque<std::thread>                 _threads;
    std::unique_ptr<tcp::acceptor>          _acceptor;
    RepoManagerUPtr                         _repoManager;
    RSAKeyManagerSPtr                       _rsaKeyManager = std::make_shared<Base::RSAKeyManager>();
};
}  /// namespace Server
