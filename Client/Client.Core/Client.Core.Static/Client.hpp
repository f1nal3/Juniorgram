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
    /// Ask for channel subscription list
    void askForChannelSubscriptionList() const;
    /**
     * @brief Ask for channel message history
     * @param channelID Channel ID
     */
    void askForMessageHistory(uint64_t channelID) const;
    /**
     * @brief Ask for reply message history
     * @param channelID
     */
    void askForReplyHistory(uint64_t channelID) const;
    /**
     * @brief Sends a none message to all clients
     */
    void messageAll() const;
    /**
     * @brief Delete user's message
     * @param messageId message ID
     */
    void userMessageDelete(const std::uint64_t messageID) const;
    /**
     * @brief Delete user's message
     * @param messageId message's text
     */
    void userMessageDelete(const std::string& messageText) const;
    /**
     * @brief Sending the ID of the subscribed channel
     * @param message ID uint64_t variable
     */
    void subscriptionChannel(const std::uint64_t channelID) const;
    /**
     * @brief Sending the name of the delete channel
     * @param channel name std::string variable
     */
    void deleteChannel(const std::string channelName) const;
    /**
     * @brief Sending the info of the created channel
     * @param channel name std::string variable
     */
    void createChannel(const std::string channelName) const;
    /**
     * @brief Send a message to server
     * @param message Message
     * @param channelID Channel ID
     */
    void storeMessage(const std::string& message, uint64_t channelID) const;
    /**
     * @brief Send a reply to server
     * @param string Message
     * @param uint64_t channelID
     * @param uint64_t msgID
     */
    void storeReply(const std::string& message, uint64_t channelID, uint64_t msgID) const;
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
    /// Message Delete Answer handler
    virtual void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState);
    /// Registration Answer handler
    virtual void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);
    /// Reply History Answer heandler
    virtual void onReplyHistoryAnswer(const std::vector<Network::ReplyInfo>& replies);
    /// Reply Store Answer handler
    virtual void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);
    /// Channel subscription Answer handler
    virtual void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode);
    /// Channel subscription list Answer handler
    virtual void onChannelSubscribingListAnswer(const std::vector<uint64_t> subscribingChannelList);
    /// Reply delete channel Answer handler
    virtual void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);
    /// Reply create channel Answer handler
    virtual void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);

private:
    asio::io_context _context;
    std::thread      _contextThread;

    bool _serverAccept = false;

    std::unique_ptr<Connection> _connection;

    SafeQueue<Message> _incomingMessagesQueue;
};
}  // namespace Network
