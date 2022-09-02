#pragma once

#include <asio.hpp>
#include <future>

#include <Utility/SafeQueue.hpp>
#include <Network/Connection.hpp>
#include <Models/Primitives.hpp>

#include <Client.hpp>
#include <ServerInfo.hpp>

namespace MockClient
{
using asio::io_context;
using asio::ip::tcp;
using Network::Connection;
using Network::Client;
using Network::Message;
using Utility::SafeQueue;

/**
*  @class MockClient.
*  @brief This class is designed to emulate a client connection.
*  @details Uses std::asio tools.
*/
class MockClient : public Client
{
protected:
    /**
    * @brief Method for signaling of disconnect from server.
    */
    void onDisconnect();

    /**
    * @brief Method for signaling of sending message to server.
    */
    void onMessageSendFailed();

    /**
    * @brief Method for signaling of login answer.
    */
    void onLoginAnswer(bool success);

    /**
    * @brief Method for signaling of server accepting.
    */
    void onServerAccepted();

    /**
    * @brief Method for signaling of server ping.
    */
    void onServerPing(double timestamp);

    /**
    * @brief Method for signaling of server message.
    */
    void onServerMessage(const uint64_t clientID);

    /**
    * @brief Method for signaling of channel list request.
    */
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels);

    /**
    * @brief Method for signaling of message history answer.
    */
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages);

    /**
    * @brief Method for signaling of message store answer.
    */
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode);

    /**
    * @brief Method for signaling of message delete answer.
    */
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState);

    /**
    * @brief Method for signaling of registration answer.
    */
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode);

    /**
    * @brief Method for signaling of reply history answer.
    */
    void onReplyHistoryAnswer(Utility::StoringReplyCodes storingReplyCode);

    /**
    * @brief Method for signaling of reply store answer.
    */
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode);

    /**
    * @brief Method for signaling of channel leave answer.
    */
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode);

    /**
    * @brief Method for signaling of channel subscription answer.
    */
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode);

    /**
    * @brief Method for signaling of channel subscription list answer.
    */
    void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList);

    /**
    * @brief Method for signaling of channel delete answer.
    */
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode);

    /**
    * @brief Method for signaling of channel create answer.
    */
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode);

    /**
    * @brief Method for signaling of direct message create answer.
    */
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer);

    /**
    * @brief Method for signaling of reaction answer.
    */
    void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState);

    /**
    * @brief Method for signaling of edit message answer.
    */
    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState);
};
}  // namespace MockClient
