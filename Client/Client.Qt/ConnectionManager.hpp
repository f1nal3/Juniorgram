#pragma once

#include <QObject>

#include "Client.hpp"
#include <Models/Primitives.hpp>
#include "ServerInfo.hpp"

/**
* @class enum class LoginState.
* @brief LoginState class for gives condition.
*/
enum class LoginState
{
    SUCCESS,
    IN_PROGRESS,
    FAILED
};

/**
* @class ReceiverManager
* @brief Class to make signal/slots possible.
*/
class ReceiverManager : public QObject
{
    Q_OBJECT
public:
    /**
    * @brief Constructor for Receiver manager.
    */
    ReceiverManager();

    /**
    * @brief Instance of receiver manager.
    */
    inline static ReceiverManager* instance() { return self; }

signals:
    /**
    * @brief Disconnect handler.
    */
    void onDisconnect();

    /**
    * @brief Login Answer handler.
    */
    void onLoginAnswer(bool success);

    /**
    * @brief Server Accepted handler.
    */
    void onServerAccepted();

    /**
    * @brief Server Ping handler.
    */
    void onServerPing(double timestamp);

    /**
    * @brief Server Message handler.
    */
    void onServerMessage(const uint64_t clientId);
 
    /**
    * @brief Channel List Request handler.
    */
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels);

    /**
    * @brief Message History Answer handler.
    */
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages);

    /**
    * @brief Message Store Answer handler.
    */
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);
 
    /**
    * @brief Registration Answer handler.
    */
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);

    /**
    * @brief User Message Delete Answer handler.
    */
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingCode);

    /**
    * @brief Reply History Answer handler.
    */
    void onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies);

    /**
    * @brief Reply Store Answer handler.
    */
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);

    /**
    * @brief Channel leave Answer handler.
    */
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode);

    /**
    * @brief Channel subscription Answer handler.
    */
    void onChannelSubscriptionAnswer(const Utility::ChannelSubscribingCodes channelSubscribingCodes);

    /**
    * @brief Channel subscription list Answer handler.
    */
    void onChannelSubscriptionListAnswer(const std::vector<uint64_t>& channelSubscriptionList);
 
    /**
    * @brief Channel delete Answer handler.
    */
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);
 
    /**
    * @brief Message edit Answer handler.
    */
    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;

    /**
    * @brief Channel create Answer handler.
    */
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);

    /**
    * @brief Direct message create Answer handler.
    */
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer);

    /**
    * @brief Reaction update answer handler.
    */
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
    ConnectionManager();

    /// \todo should be in Client Core
    /**
    * @brief LoginState class for gives condition.
    */
    inline static LoginState loginState;

    /**
    * @brief Initialize connection to server.
    */
    void init();

    /**
    * @brief Default behavior.
    */
    ~ConnectionManager() override = default;

protected:
    /**
    * @brief  Disconnect handler.
    */
    void onDisconnect() override;

    /**
    * @brief Login Answer handler.
    */
    void onLoginAnswer(bool success) override;

    /**
    * @brief Server Accepted handler.
    */
    void onServerAccepted() override;

    /**
    * @brief Server Ping handler.
    */
    void onServerPing(double timestamp) override;

    /**
    * @brief Server Message handler.
    */
    void onServerMessage(const uint64_t clientId) override;

    /**
    * @brief Channel List Request handler.
    */
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) override;

    /**
    * @brief Message History Answer handler.
    */ 
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) override;

    /**
    * @brief Message Store Answer handler.
    */ 
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;

    /**
    * @brief Registration Answer handler.
    */ 
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;

    /**
    * @brief User Message Delete Answer handler.
    */ 
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) override;

    /**
    * @brief Reply History Answer handler.
    */ 
    void onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies) override;

    /**
    * @brief Reply Store Answer handler.
    */ 
    void onReplyStoreAnswer(const Utility::StoringReplyCodes storingReplyCode) override;

    /**
    * @brief Channel leave Answer handler.
    */ 
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes ChannelLeaveCode) override;

    /**
    * @brief Channel subscription Answer handler.
    */ 
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) override;

    /**
    * @brief Channel subscription list Answer handler.
    */
    void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList) override;

    /**
    * @brief Channel delete Answer handler.
    */
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) override;

    /*
    * @brief Message edit Answer handler.
    */
    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const override;

    /**
    * @brief Channel create Answer handler.
    */
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) override;

    /**
    * @brief Direct message create Answer handler.
    */
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) override;

    /**
    * @brief Reaction update answer handler.
    */ 
    void onMessageReactionAnswer(const Utility::ReactionMessageCodes reactionCode) override;

    /**
    * @brief configurator of connection properties.
    */ 
    void configureConnectionProperties() const;
};
