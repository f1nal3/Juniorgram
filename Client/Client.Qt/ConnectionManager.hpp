#pragma once

#include <QObject>

#include "Client.hpp"
#include "Network/Primitives.hpp"
#include "ServerInfo.hpp"

/// Login status
enum class LoginState
{
    SUCCESS,
    IN_PROGRESS,
    FAILED
};

/**
 * @class ReceiverManager
 * @brief Class to make signal/slots possible
 */
class ReceiverManager : public QObject
{
    Q_OBJECT
public:
    /// Constructor for Receiver manager
    ReceiverManager();
    /// Instance of receiver manager
    inline static ReceiverManager* instance() { return self; }

signals:
    /// Disconnect handler
    void onDisconnect();

    /// Login Answer handler
    void onLoginAnswer(bool success);
    /// Server Accepted handler
    void onServerAccepted();
    /// Server Ping handler
    void onServerPing(double timestamp);
    /// Server Message handler
    void onServerMessage(const uint64_t clientId);
    /// Channel List Request handler
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels);
    /// Message History Answer handler
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages);
    /// Message Store Answer handler
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);
    /// RegistrationPage Answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);
    /// User Message Delete Answer handler
    void onUserMessageDeleteAnswer();

private:
    static ReceiverManager* self;
};

/**
 * @class ConnectionManager
 * @brief Class to send messages to server and handle message from server
 */
class ConnectionManager : public Network::Client
{
public:
    /// TODO: should be in client core
    /// Login status
    inline static LoginState loginState;

    /// Initialize connection to server
    void init() { connectToServer(ServerInfo::address, ServerInfo::port); }
    /// Default behavior
    virtual ~ConnectionManager() = default;

protected:
    /// Disconnect handler
    void onDisconnect() override;

    /// Login Answer handler
    void onLoginAnswer(bool success) override;
    /// Server Accepted handler
    void onServerAccepted() override;
    /// Server Ping handler
    void onServerPing(double timestamp) override;
    /// Server Message handler
    void onServerMessage(const uint64_t clientId) override;
    /// Channel List Request handler
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels) override;
    /// Message History Answer handler
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages) override;
    /// Message Store Answer handler
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;
    /// RegistrationPage Answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;
    /// User Message Delete Answer handler
    void onUserMessageDeleteAnswer() override;
};
