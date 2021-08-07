#pragma once

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

namespace Network
{
/**
 * @class Client
 * @brief Class for handling connection
 */
class Client
{
public:
    /// Disconnect from server
    ~Client();

    /// Connect to server with IP(host) and Port(port)
    bool connectToServer(const std::string_view& host, const uint16_t port);
    /// Disconnect from server
    void disconnectFromServer();

    /// Returns true if connection is established
    [[nodiscard]] bool isConnected() const;

    /// Sends a message to server
    void send(const Message& message) const;

    /// Incoming message queue
    SafeQueue<Message>& incoming() { return _incomingMessagesQueue; }

    /// Handler
    void loop();

    /// Ping a server
    void pingServer() const;

    /// Ask for channel list
    void askForChannelList() const;

    /**
     * @brief Ask for channel message history
     * @param channelID Channel ID
     */
    void askForMessageHistory(uint64_t channelID) const;
    /**
     * @brief Sends a none message to all clients
     */
    void messageAll() const;
    /**
     * @brief Delete message from user
     * @param userId user ID
     * @param messageId message ID
     */
    void userMessageDelete(uint64_t userId, uint64_t messageId) const;
    /**
     * @brief Send a message to server
     * @param message Message
     * @param channelID Channel ID
     */
    void storeMessage(const std::string& message, uint64_t channelID) const;

    /**
     * @brief Send an registration request to server
     * @param email User E-Mail
     * @param login User Login
     * @param password Password hashed
     */
    void userRegistration(const std::string& email, const std::string& login, const std::string& password) const;
    /**
     * @brief Send an authorization request to server
     * @param login User login
     * @param password User password
     */
    void userAuthorization(const std::string& login, const std::string& password) const;

protected:
    /// Disconnect handler
    virtual void onDisconnect();
    /// Message send failure handler
    virtual void onMessageSendFailed(const Message& message) const;

    /// Login Answer handler
    virtual void onLoginAnswer(bool success);
    /// Server Accepted handler
    virtual void onServerAccepted();
    /// Server Ping handler
    virtual void onServerPing(double timestamp);
    /// Server Message handler
    virtual void onServerMessage(const uint64_t clientId);
    /// Channel List Request handler
    virtual void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels);
    /// Message History Answer handler
    virtual void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages);
    /// Message Store Answer handler
    virtual void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);
    /// Registration Answer handler
    virtual void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);
    /// User Message Delete Answer handler
    virtual void onUserMessageDeleteAnswer();

private:
    asio::io_context _context;
    std::thread      _contextThread;

    std::unique_ptr<Connection> _connection;

    SafeQueue<Message> _incomingMessagesQueue;
};
}  // namespace Network
