#pragma once

#include "Client.hpp"
#include "ServerInfo.hpp"

namespace MockObject
{
using Network::Connection;
using Network::Client;
using Network::Message;

/**
* @brief MockClient class.
* @details Mock client is intended to indicate the actions processed by the server. /
*          The response methods to a certain message are redefined.
*/
class MockClient : public Client
{
public:
    ~MockClient() override = default;

protected:
    /**
    * @brief Method for signaling of disconnect from server.
    */
    void onDisconnect() override;

    /**
    * @brief Method for signaling of sending message to server.
    */
    void onMessageSendFailed(const Message& message) const override;

    /**
    * @brief Method for signaling of login answer.
    */
    void onLoginAnswer(bool success) override;

    /**
    * @brief Method for signaling of server accepting.
    */
    void onServerAccepted() override;

    /**
    * @brief Method for signaling of server ping.
    */
    void onServerPing(double timestamp) override;

    /**
    * @brief Method for signaling of server message.
    */
    void onServerMessage(const uint64_t clientID) override;

    /**
    * @brief Method for signaling of channel list request.
    */
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) override;

    /**
    * @brief Method for signaling of message history answer.
    */
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) override;

    /**
    * @brief Method for signaling of message store answer.
    */
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) override;

    /**
    * @brief Method for signaling of message delete answer.
    */
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) override;

    /**
    * @brief Method for signaling of registration answer.
    */
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) override;

    /**
    * @brief Method for signaling of reply history answer.
    */
    void onReplyHistoryAnswer(const std::vector<Models::ReplyInfo>& replies) override;

    /**
    * @brief Method for signaling of reply store answer.
    */
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode) override;

    /**
    * @brief Method for signaling of channel leave answer.
    */
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode) override;

    /**
    * @brief Method for signaling of channel subscription answer.
    */
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) override;

    /**
    * @brief Method for signaling of channel subscription list answer.
    */
    void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList) override;

    /**
    * @brief Method for signaling of channel delete answer.
    */
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) override;

    /**
    * @brief Method for signaling of channel create answer.
    */
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) override;

    /**
    * @brief Method for signaling of direct message create answer.
    */
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) override;

    /**
    * @brief Method for signaling of reaction answer.
    */
    void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState) override;

    /**
    * @brief Method for signaling of edit message answer.
    */
    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const override;
};
}  /// namespace MockObject
