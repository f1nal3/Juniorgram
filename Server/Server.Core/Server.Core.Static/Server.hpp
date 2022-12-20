#pragma once

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include <FileLogger.hpp>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Utility/SafeQueue.hpp"

namespace DataAccess
{
class PostgreRepositoryManager;
}

namespace Server
{
using asio::ip::tcp;
using Network::Message;
using Utility::SafeQueue;
using Network::Connection;
using Base::Logger::LogLevel;
using Base::Logger::FileLogger;
using DataAccess::PostgreRepositoryManager;

/**
 *  @class Server class
 *  @brief This class does all logic which is needed to run the server.
 *  @details Uses std::asio tools.
 */
class Server
{
private:
    uint64_t _idCounter         = 10000;
    uint64_t _criticalQueueSize = 100;
    uint64_t _newThreadsCount   = std::thread::hardware_concurrency();

    asio::io_context                                       _context;
    tcp::acceptor                                          _acceptor;
    std::deque<std::shared_ptr<Connection>>                _connectionsPointers;
    SafeQueue<Message>                                     _incomingMessagesQueue;
    std::deque<std::thread>                                _threads;
    std::unique_ptr<PostgreRepositoryManager>              _postgreManager;

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

public:
    /**
    * @brief Explicit constructor
    * @param const uint16_t& port
    * @details Uses incoming port value from ArgumentParser.
    */
    explicit Server(const uint16_t& port);

    Server(const uint16_t& port, std::unique_ptr<PostgreRepositoryManager> RepMngr) noexcept;

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
     * @bried Method for accepting to a client, works asynchronously.
     */
    void acceptingClientConnection(const std::error_code& error, asio::ip::tcp::socket& socket);

    /**
     * @brief Method for sending the message from client.
     * @param Connection management class as std::shared_ptr<Network::Connection>& and Network::Message& class.
     */
    void messageClient(std::shared_ptr<Connection> client, const Message& message);

    /**
     * @brief Method of sending a message to all clients.
     * @param Network::Message& class and connection management class as std::shared_ptr<Network::Connection>&.
     */
    void messageAllClients(const Message& message, const std::shared_ptr<Connection>& exceptionClient = nullptr);

    /**
     * @brief Method for updating messages.
     * @param std::size_t limit and bool for method wait() in SafeQueue.
     */
    void update(std::size_t maxMessages = std::numeric_limits<size_t>::max(), bool wait = true);

    /**
     * @brief The method for checking of server ping.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void checkServerPing(const std::shared_ptr<Connection>& client, const Message& message) const;

    /**
     * @brief The method for checking of receiving all messages by the client.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void readAllMessage(const std::shared_ptr<Connection>& client, const Message& message);

    /**
     * @brief The method of checking for a channel list.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     */
    void channelListRequest(const std::shared_ptr<Connection>& client) const;

    /**
     * @brief The method for checking message history extraction.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageHistoryRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the retrieval of stored messages.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageStoreRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking answer history extraction.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void replyHistoryRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking repeatly the retrieval of stored messages.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void replyStoreRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking of deletion a message request.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the editing of a query.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageEditRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the reaction.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void messageReactionRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the registration request.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void registrationRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the logging request.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void loginRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the exit from the channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelLeaveRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for verifying a channel subscription.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelSubscribeRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking the verification of users subscribed to a channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     */
    void channelSubscriptionListRequest(const std::shared_ptr<Connection>& client) const;

    /**
     * @brief The method of checking the deletion of a channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelDeleteRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method of checking the creation of a channel.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void channelCreateRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for checking direct messages received by the user.
     * @param const std::shared_ptr<Connection>& client - for connect messages to the client or server.
     * @param Message& message - for use the message header functionality.
     */
    void directMessageCreateRequest(const std::shared_ptr<Connection>& client, Message& message) const;

    /**
     * @brief The method for getting default request.
     */
    void defaultRequest() const;
};
}  // namespace Server
