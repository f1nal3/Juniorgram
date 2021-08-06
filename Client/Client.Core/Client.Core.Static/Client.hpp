#pragma once

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/SafeQueue.hpp"

namespace Network
{
class Client
{
public:
    ~Client();

    /// Connect to server with IP(host) and Port(port)
    bool connectToServer(const std::string_view& host, const uint16_t port);
    /// Disconnect from server
    void disconnectFromServer();

    [[nodiscard]] bool isConnected() const;

    void send(const Message& message) const;

    SafeQueue<Message>& incoming() { return _incomingMessagesQueue; }

    void loop();

    void pingServer() const;

    void askForChannelList() const;

    void askForMessageHistory(uint64_t channelID) const;
    void messageAll() const;
    void userMessageDelete(uint64_t userId, uint64_t messageId) const;
    void storeMessage(const std::string& message, uint64_t channelID) const;

    void userRegistration(const std::string& email, const std::string& login, const std::string& password) const;
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
