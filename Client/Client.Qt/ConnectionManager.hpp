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
    /// Registration Answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);
    /// User Message Delete Answer handler
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingCode);
    /// Reply History Answer handler
    void onReplyHistoryAnswer(const std::vector<Network::ReplyInfo>& replies);
    /// Reply Store Answer handler
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);
    /// Channel leave Answer handler
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode);
    /// Channel subscription Answer handler
    void onChannelSubscriptionAnswer(const Utility::ChannelSubscribingCodes channelSubscribingCodes);
    /// Channel subscription list Answer handler
    void onChannelSubscriptionListAnswer(const std::vector<uint64_t> channelSubscriptionList);
    /// Channel delete Answer handler
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);
    /// Channel create Answer handler
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);
    /// Direct message create Answer handler
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer);
    /// Reaction update answer handler
    void onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionCode);

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
    /// \todo: should be in client core
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
    /// Registration Answer handler
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;
    /// User Message Delete Answer handler
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) override;
    /// Reply History Answer handler
    void onReplyHistoryAnswer(const std::vector<Network::ReplyInfo>& replies) override;
    /// Reply Store Answer handler
    void onReplyStoreAnswer(const Utility::StoringReplyCodes storingReplyCode) override;
    /// Channel leave Answer handler
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode) override;
    /// Channel subscription Answer handler
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) override;
    /// Channel subscription list Answer handler
    void onChannelSubscribingListAnswer(const std::vector<uint64_t> subscribingChannelList) override;
    /// Channel delete Answer handler
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) override;
    /// Channel create Answer handler
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) override;
    /// Direct message create Answer handler
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) override;
    /// Reaction update answer handler
    void onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionCode) override;
};
