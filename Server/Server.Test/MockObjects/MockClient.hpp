#pragma once

#include <iostream>
#include <string>

#include <asio.hpp>
#include <future>

#include <Utility/UtilityTime.hpp>
#include <Utility/SafeQueue.hpp>
#include <Network/Connection.hpp>
#include <Models/Primitives.hpp>
#include <Client.Core/Client.Core.Static/ServerInfo.hpp>

#include <MockPostgreRepository.hpp>

namespace MockClient
{
using asio::io_context;
using asio::ip::tcp;
using Network::Connection;
using Models::LoginInfo;
using Network::Message;
using Models::MessageInfo;
using Models::RegistrationInfo;
using Models::ReplyInfo;
using Utility::SafeQueue;
using RTC = std::chrono::system_clock;
using UtilityTime::timestamp_t;

/**
*  @class MockClient.
*  @brief This class is designed to emulate a client connection.
*  @details Uses std::asio tools.
*/
class MockClient
{
public:
    /**
    * @brief Method for connecting to server.
    * @param host means as ip address.
    */
    bool connectToServer(const std::string_view& host, const uint16_t port);

    /**
    * @brief Method for disconnecting from server.
    */
    void disconnectFromServer();

    /**
    * @brief Method for checking connection to server.
    */
    bool isConnected() const;

    /**
    * @brief Method for sending message to server.
    */
    void send(const Message& message) const;

    /**
    * @brief Method for accepting & processing messages.
    */
    void loop();

    /**
    * @brief Method for getting queue with messages;
    */
    SafeQueue<Message>& getMessage();

protected:

    /**
    * @brief Method for signaling of disconnect from server.
    */
    void onDisconnect() const;

    /**
    * @brief Method for signaling of sending message to server.
    */
    void onMessageSendFailed() const;

    /**
    * @brief Method for signaling of login answer.
    */
    void onLoginAnswer(bool success) const;
 
    /**
    * @brief Method for signaling of server accepting.
    */
    void onServerAccepted() const;

    /**
    * @brief Method for signaling of server ping.
    */
    void onServerPing(double timestamp) const;

    /**
    * @brief Method for signaling of server message.
    */
    void onServerMessage(const uint64_t clientID) const;

    /**
    * @brief Method for signaling of channel list request.
    */
    void onChannelListRequest(const std::vector<Models::ChannelInfo>& channels) const;

    /**
    * @brief Method for signaling of message history answer.
    */
    void onMessageHistoryAnswer(const std::vector<Models::MessageInfo>& messages) const;

    /**
    * @brief Method for signaling of message store answer.
    */
    void onMessageStoreAnswer(Utility::StoringMessageCodes storingMessageCode) const;

    /**
    * @brief Method for signaling of message delete answer.
    */
    void onUserMessageDeleteAnswer(const Utility::DeletingMessageCodes deletingState) const;

    /**
    * @brief Method for signaling of registration answer.
    */
    void onRegistrationAnswer(Utility::RegistrationCodes registrationCode) const;

    /**
    * @brief Method for signaling of reply history answer.
    */
    void onReplyHistoryAnswer(Utility::StoringReplyCodes storingReplyCode) const;

    /**
    * @brief Method for signaling of reply store answer.
    */
    void onReplyStoreAnswer(Utility::StoringReplyCodes storingReplyCode) const;

    /**
    * @brief Method for signaling of channel leave answer.
    */
    void onChannelLeaveAnswer(Utility::ChannelLeaveCodes channelLeaveCode) const;

    /**
    * @brief Method for signaling of channel subscription answer.
    */
    void onChannelSubscribingAnswer(const Utility::ChannelSubscribingCodes subscribingChannelCode) const;

    /**
    * @brief Method for signaling of channel subscription list answer.
    */
    void onChannelSubscribingListAnswer(const std::vector<uint64_t>& subscribingChannelList) const;

    /**
    * @brief Method for signaling of channel delete answer.
    */
    void onChannelDeleteAnswer(Utility::ChannelDeleteCode channelDeleteCode) const;

    /**
    * @brief Method for signaling of channel create answer.
    */
    void onChannelCreateAnswer(Utility::ChannelCreateCodes channelCreateCode) const;

    /**
    * @brief Method for signaling of direct message create answer.
    */
    void onDirectMessageCreateAnswer(Utility::DirectMessageStatus directMessageCreateAnswer) const;

    /**
    * @brief Method for signaling of reaction answer.
    */
    void onMessageReactionAnswer(Utility::ReactionMessageCodes reactionState) const;

     /**
     * @brief Method for signaling of edit message answer.
     */
    void onEditMessageAnswer(Utility::EditingMessageCodes reactionState) const;

private:
    io_context                  testContext;
    std::thread                 testContextThread;
    std::unique_ptr<Connection> testConnection;
    SafeQueue<Message>          testIncomingMessagesQueue;

    bool serverAccept = false;
};
}  // namespace MockClient
