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

    bool connect(const std::string_view& host, uint16_t port);
    void disconnect();

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

    // Handling functions
    virtual void onLoginAnswer(bool success)
    {
        (void)(success);
        std::cerr << "[Client][Warning] login answer is not implemented\n";
    }
    virtual void onServerAccepted() { std::cerr << "[Client][Warning] server accepted is not implemented\n"; }
    virtual void onServerPing(double timestamp)
    {
        (void)(timestamp);
        std::cerr << "[Client][Warning] server ping is not implemented\n";
    }
    virtual void onServerMessage(const uint64_t clientId)
    {
        (void)(clientId);
        std::cerr << "[Client][Warning] server message is not implemented\n";
    }
    virtual void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels)
    {
        (void)(channels);
        std::cerr << "[Client][Warning] channel list request is not implemented\n";
    }
    virtual void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages)
    {
        (void)(messages);
        std::cerr << "[Client][Warning] message history answer is not implemented\n";
    }
    virtual void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode)
    {
        (void)(storingMessageCode);
        std::cerr << "[Client][Warning] message store answer is not implemented\n";
    }
    virtual void onRegistrationAnswer(Utility::RegistrationCodes registrationCode)
    {
        (void)(registrationCode);
        std::cerr << "[Client][Warning] registration answer is not implemented\n";
    }
    virtual void onUserMessageDeleteAnswer() { std::cerr << "[Client][Warning] user message delete answer is not implemented\n"; }

private:
    asio::io_context _context;
    std::thread      _contextThread;

    std::unique_ptr<Connection> _connection;

    SafeQueue<Message> _incomingMessagesQueue;
};
}  // namespace Network
