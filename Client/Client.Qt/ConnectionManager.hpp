#pragma once

#include "Client.hpp"
#include "Network/Primitives.hpp"
#include "ServerInfo.hpp"
#include "Widgets/ChannelListWindow.hpp"

enum class LoginState
{
    SUCCESS,
    IN_PROGRESS,
    FAILED
};

class ReceiverManager : public QObject
{
    Q_OBJECT
public:
    ReceiverManager();
    inline static ReceiverManager* instance() { return self; }

signals:
    void onServerAccepted();
    void onServerPing(double timestamp);
    void onServerMessage(const uint64_t clientId);
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels);
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages);
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);
    void onUserMessageDeleteAnswer();
    void onLoginAnswer(bool success);
    void onDisconnect();

private:
    static ReceiverManager* self;
};

class ConnectionManager : public Network::Client
{
public:
    inline static LoginState loginState;

    void init() { connectToServer(ServerInfo::address, ServerInfo::port); }

protected:
    void onServerAccepted() override;
    void onServerPing(double timestamp) override;
    void onServerMessage(const uint64_t clientId) override;
    void onChannelListRequest(const std::vector<Network::ChannelInfo>& channels) override;
    void onMessageHistoryAnswer(const std::vector<Network::MessageInfo>& messages) override;
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;
    void onUserMessageDeleteAnswer() override;
    void onLoginAnswer(bool success) override;
    void onDisconnect() override;
};
